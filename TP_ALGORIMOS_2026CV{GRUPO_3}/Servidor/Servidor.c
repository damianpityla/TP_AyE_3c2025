#include "Servidor.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int iniciar_entorno_socket()
{
    WSADATA wsa;
    return WSAStartup(MAKEWORD(2, 2), &wsa);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SOCKET conectar_servidor(const char* ip, int puerto)
{
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server;

    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_family = AF_INET;
    server.sin_port = htons(puerto);

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        return INVALID_SOCKET;
    }
    return sock;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int enviar_partida(SOCKET sock, int id_jugador, float puntos, int movimientos)
{
    char buffer[256];
    int bytes;
    sprintf(buffer, "PARTIDA %d %.2f %d", id_jugador, puntos, movimientos);
    send(sock, buffer, strlen(buffer), 0);

    bytes = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if(bytes > 0)
    {
        buffer[bytes] = '\0';
        return strstr(buffer, "GUARDADA") != NULL;
    }
    return 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void procesar_peticiones_pendientes(tCola* pCola, SOCKET client_sock, tArbolBin *pIndice)
{
    char peticion[1024];
    char respuesta[1024];
    char nombre[50];
    tArbolBin *nodo;
    tNodoLista* actual;
    tIndiceJugador clave;
    tRanking reg;
    tLista ranking;

    while(SacarDeCola(pCola, peticion, sizeof(peticion)) == TODO_OK)
    {
        if (strncmp(peticion, "LOGIN", 5) == 0)
        {
            sscanf(peticion + 6, "%s", nombre);
            strcpy(clave.nombre, nombre);
            nodo = BusquedaEnArbolBin(pIndice, &clave, cmpIndicePorNombre);
            if (nodo && *nodo)
            {
                sprintf(respuesta, "OK %d", ((tIndiceJugador*)(*nodo)->Info)->id);
            }
            else
            {
                sprintf(respuesta, "ERROR_USER_NOT_FOUND");
            }
            send(client_sock, respuesta, (int)strlen(respuesta), 0);
        }
        else if (strncmp(peticion, "REGISTER", 8) == 0)
        {
            sscanf(peticion + 9, "%s", nombre);
            strcpy(clave.nombre, nombre);
            nodo = BusquedaEnArbolBin(pIndice, &clave, cmpIndicePorNombre);
            if (nodo && *nodo)
            {
                sprintf(respuesta, "ERROR_ALREADY_EXISTS");
            }
            else
            {
                if (altaJugadorServidor(ARCH_INDICE, pIndice, nombre, cmpIndicePorNombre) == TODO_OK)
                {
                    sprintf(respuesta, "REGISTER_OK");
                }
                else
                {
                    sprintf(respuesta, "ERROR_DB");
                }
            }
            send(client_sock, respuesta, (int)strlen(respuesta), 0);
        }
        else if (strncmp(peticion, "PARTIDA", 7) == 0)
        {
            int id, movs;
            float pts;
            sscanf(peticion + 8, "%d %f %d", &id, &pts, &movs);
            if (registrarPartidaEnServidor(ARCH_PARTIDAS, id, pts, movs) == TODO_OK)
            {
                send(client_sock, "PARTIDA GUARDADA", 16, 0);
            }
        }
        else if (strncmp(peticion, "RANKING", 7) == 0)
        {
            if (GenerarRanking(ARCH_PARTIDAS, pIndice, &ranking) == TODO_OK)
            {
                actual = ranking;
                while(actual)
                {
                    memcpy(&reg, actual->Info, sizeof(tRanking));
                    send(client_sock, (char*)&reg, sizeof(tRanking), 0);
                    actual = actual->sig;
                }
                VaciarLista(&ranking);
            }
            memset(&reg, 0, sizeof(tRanking));
            reg.idJugador = -1;
            send(client_sock, (char*)&reg, sizeof(tRanking), 0);
        }
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ejecutar_servidor(int puerto, tArbolBin* pIndice)
{
    SOCKET server_fd, nuevo_socket;
    struct sockaddr_in direccion;
    int addrlen = sizeof(direccion);
    tCola colaPeticiones;

    CrearCola(&colaPeticiones);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET) return;

    direccion.sin_family = AF_INET;
    direccion.sin_addr.s_addr = INADDR_ANY;
    direccion.sin_port = htons(puerto);

    if(bind(server_fd, (struct sockaddr *)&direccion, sizeof(direccion)) == SOCKET_ERROR)
        return;

    listen(server_fd, 3);

    printf("Servidor de Base de Datos escuchando en puerto %d...\n", puerto);

    while(1)
    {
        nuevo_socket = accept(server_fd, (struct sockaddr *)&direccion, &addrlen);

        if (nuevo_socket != INVALID_SOCKET)
        {
            char buffer[1024];
            int bytes;
            while((bytes = recv(nuevo_socket, buffer, sizeof(buffer)-1, 0)) > 0)
            {
                buffer[bytes] = '\0';
                PonerEnCola(&colaPeticiones, buffer, bytes + 1);
                procesar_peticiones_pendientes(&colaPeticiones, nuevo_socket, pIndice);
            }
            closesocket(nuevo_socket);
        }
    }
}
int cmpIndicePorNombre(const void *a, const void *b) {
    const tIndiceJugador *ia = (const tIndiceJugador*)a;
    const tIndiceJugador *ib = (const tIndiceJugador*)b;
    return strcmp(ia->nombre, ib->nombre);
}
int GenerarRanking(const char* archPartidas, tArbolBin* pIndice, tLista* pRanking)
{
    FILE* pf;
    tPartida part;
    tRanking reg;
    tNodoArbolBin** nodo;
    tNodoLista* listaTemporal;
    tNodoLista* elim;

    pf = fopen(archPartidas, "rb");
    if(!pf)
    {
        return ERROR_ARCHIVO;
    }

    listaTemporal = NULL;
    CrearLista(pRanking);

    while(fread(&part, sizeof(tPartida), 1, pf) == 1)
    {
        nodo = BuscarPorIdEnArbol(pIndice, part.idjugador);
        if(nodo && *nodo)
        {
            strcpy(reg.Nombre, ((tIndiceJugador*)(*nodo)->Info)->nombre);
            reg.idJugador = part.idjugador;
            reg.TotalPuntos = part.Puntuacion;
            reg.partidas = 1;

            InsertarEnOrdenLista(&listaTemporal, &reg, sizeof(tRanking), SIN_DUPLICADOS, cmpRankingPorId, AcumularRanking);
        }
    }
    fclose(pf);

    while(listaTemporal)
    {
        memcpy(&reg, listaTemporal->Info, sizeof(tRanking));
        InsertarEnOrdenLista(pRanking, &reg, sizeof(tRanking), CON_DUPLICADOS, cmpRankingPorPuntosDesc, NULL);

        elim = listaTemporal;
        listaTemporal = listaTemporal->sig;
        free(elim->Info);
        free(elim);
    }

    return TODO_OK;
}
int altaJugadorServidor(const char* archIndice, tArbolBin* pIndice, const char* nombre, tCmp cmp)
{
    FILE* pf;
    tJugador nuevo;
    tIndiceJugador ind;

    pf = fopen("jugadores.dat", "ab+");
    if(!pf)
    {
        return ERROR_ARCHIVO;
    }

    fseek(pf, 0, SEEK_END);
    nuevo.id = (int)(ftell(pf) / sizeof(tJugador)) + 1;
    strncpy(nuevo.nombre, nombre, TAM_NOMBRE);
    nuevo.nombre[TAM_NOMBRE] = '\0';

    fwrite(&nuevo, sizeof(tJugador), 1, pf);
    fclose(pf);

    ind.id = nuevo.id;
    ind.desplazamiento = nuevo.id - 1;
    strcpy(ind.nombre, nuevo.nombre);

    InsertarEnArbolBin(pIndice, &ind, sizeof(tIndiceJugador), cmp);
    GuardarIndiceBinario(*pIndice, archIndice);

    return TODO_OK;
}

int registrarPartidaEnServidor(const char* arch, int id, float pts, int movs) {
    FILE* pf = fopen(arch, "ab");
    if (!pf) return ERROR_ARCHIVO;

    tPartida p;
    p.idjugador = id;
    p.Puntuacion = pts;
    p.CantMovimientos = movs;

    fwrite(&p, sizeof(tPartida), 1, pf);
    fclose(pf);
    return TODO_OK;
}
void EscribirABBEnArchivo(tNodoArbolBin* raiz, FILE* arch)
{
    if(raiz == NULL)
        return;

    fwrite(raiz->Info, sizeof(tIndiceJugador), 1, arch);
    EscribirABBEnArchivo(raiz->Izq, arch);
    EscribirABBEnArchivo(raiz->Der, arch);
}
tNodoArbolBin** BuscarPorIdEnArbol(tArbolBin* p, int id)
{
    tNodoArbolBin** izq;
    if(!*p)
        return NULL;

    if(((tIndiceJugador*)(*p)->Info)->id == id)
        return p;

    izq = BuscarPorIdEnArbol(&(*p)->Izq, id);
    if(izq)
        return izq;

    return BuscarPorIdEnArbol(&(*p)->Der, id);
}
void GuardarIndiceBinario(tArbolBin raiz, const char* nombreArchivo)
{
    FILE* arch;

    arch = fopen(nombreArchivo, "wb");
    if(!arch)
    {
        return;
    }

    EscribirABBEnArchivo(raiz, arch);
    fclose(arch);
}
void CargarIndiceBinario(tArbolBin* pIndice, const char* nombreArchivo)
{
    FILE* arch = fopen(nombreArchivo, "rb");
    if(!arch)
    {
        printf("[SERVIDOR] No se encontro archivo de indice. Iniciando limpio.\n");
        return;
    }

    tIndiceJugador reg;
    fread(&reg, sizeof(tIndiceJugador), 1, arch);
    while(!feof(arch))
    {
        InsertarEnArbolBin(pIndice, &reg, sizeof(tIndiceJugador), cmpIndicePorNombre);
        fread(&reg, sizeof(tIndiceJugador), 1, arch);
    }

    fclose(arch);
    printf("[SERVIDOR] Indice cargado exitosamente desde %s\n", nombreArchivo);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int cmpRankingPorId(const void *a, const void *b)
{
    tRanking *ra;
    tRanking *rb;

    ra = (tRanking*)a;
    rb = (tRanking*)b;

    return ra->idJugador - rb->idJugador;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AcumularRanking(void *nodoInfo, const void *elem)
{
    tRanking* existente = (tRanking*)nodoInfo;
    const tRanking *nuevo = (const tRanking*)elem;

    existente->TotalPuntos += nuevo->TotalPuntos;
    existente->partidas += 1;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int cmpRankingPorPuntosDesc(const void *a, const void *b)
{
    tRanking *ra;
    tRanking *rb;

    ra = (tRanking*)a;
    rb = (tRanking*)b;

    if (rb->TotalPuntos > ra->TotalPuntos)
    {
        return 1;
    }

    if (rb->TotalPuntos < ra->TotalPuntos)
    {
        return -1;
    }

    return strcmp(ra->Nombre, rb->Nombre);
}
