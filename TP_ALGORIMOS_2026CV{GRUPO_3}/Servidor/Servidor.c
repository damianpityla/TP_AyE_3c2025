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

    if(connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        return INVALID_SOCKET;
    }
    return sock;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int enviar_partida(SOCKET sock, int id_jugador, float puntos, int movimientos)
{
    char buffer[TAM_BUFFER];
    int bytes;

    sprintf(buffer, "PARTIDA %d %.2f %d", id_jugador, puntos, movimientos);
    send(sock, buffer, strlen(buffer), 0);

    bytes = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if(bytes > 0)
    {
        buffer[bytes] = '\0';
        if(strstr(buffer, "GUARDADA") != NULL)
            return ENVIO_OK;
    }
    return ENVIO_ERROR;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void procesar_peticiones_pendientes(tCola *pCola, SOCKET client_sock, tArbolBin *pIndice)
{
    char peticion[TAM_PETICION];
    char respuesta[TAM_RESPUESTA];
    char nombre[TAM_NOMBRE];
    tArbolBin *nodo;
    tNodoLista *actual;
    tIndiceJugador clave;
    tRanking reg;
    tLista ranking;
    int id, movs;
    float pts;

    while(SacarDeCola(pCola, peticion, sizeof(peticion)) == TODO_OK)
    {
        if (strncmp(peticion, CMD_LOGIN, (int)strlen(CMD_LOGIN)) == 0)
        {
            sscanf(peticion + strlen(CMD_LOGIN), "%s", nombre);
            strcpy(clave.nombre, nombre);
            nodo = BuscarPorNombreEnArbol(pIndice, nombre);
            if (nodo && *nodo)
            {
                sprintf(respuesta, "OK %d", ((tIndiceJugador*)(*nodo)->Info)->id);
            }
            else
            {
                sprintf(respuesta, MSJ_ERR_USER_NOT_FOUND);
            }
            send(client_sock, respuesta, (int)strlen(respuesta), 0);
        }
        else if (strncmp(peticion, CMD_REGISTER, (int)strlen(CMD_REGISTER)) == 0)
        {
            sscanf(peticion + strlen(CMD_REGISTER), "%s", nombre);
            strcpy(clave.nombre, nombre);
            nodo = BuscarPorNombreEnArbol(pIndice, nombre);
            if (nodo && *nodo)
            {
                sprintf(respuesta, MSJ_ERR_ALREADY_EXISTS);
            }
            else
            {
                if (altaJugadorServidor(ARCH_INDICE, pIndice, nombre, cmpIndicePorId) == TODO_OK)
                {
                    sprintf(respuesta, MSJ_REGISTER_OK);
                }
                else
                {
                    sprintf(respuesta, MSJ_ERR_DB);
                }
            }
            send(client_sock, respuesta, (int)strlen(respuesta), 0);
        }
        else if (strncmp(peticion, CMD_PARTIDA, (int)strlen(CMD_PARTIDA)) == 0)
        {
            if (sscanf(peticion + strlen(CMD_PARTIDA), "%d %f %d", &id, &pts, &movs) == 3)
            {
                if (registrarPartidaEnServidor(ARCH_PARTIDAS, id, pts, movs) == TODO_OK)
                {
                    send(client_sock, MSJ_OK_PARTIDA, strlen(MSJ_OK_PARTIDA), 0);
                }
            }
        }
        else if (strncmp(peticion, CMD_RANKING, (int)strlen(CMD_RANKING)) == 0)
        {
            CrearLista(&ranking);
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
void ejecutar_servidor(int puerto, tArbolBin *pIndice)
{
    SOCKET server_fd, nuevo_socket;
    struct sockaddr_in direccion;
    int addrlen = sizeof(direccion);
    tCola colaPeticiones;
    int bytes;
    int ejecutando = 1;
    char buffer[TAM_BUFFER];

    CrearCola(&colaPeticiones);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd == INVALID_SOCKET)
        return;

    direccion.sin_family = AF_INET;
    direccion.sin_addr.s_addr = INADDR_ANY;
    direccion.sin_port = htons(puerto);

    if(bind(server_fd, (struct sockaddr *)&direccion, sizeof(direccion)) == SOCKET_ERROR)
    {
        closesocket(server_fd);
        return;
    }

    listen(server_fd, SRV_BACKLOG);

    printf("Servidor de Base de Datos escuchando en puerto %d...\n", puerto);

    while(ejecutando)
    {
        nuevo_socket = accept(server_fd, (struct sockaddr *)&direccion, &addrlen);

        if (nuevo_socket != INVALID_SOCKET)
        {
            while((bytes = recv(nuevo_socket, buffer, TAM_BUFFER -1, 0)) > 0)
            {
                buffer[bytes] = '\0';
                PonerEnCola(&colaPeticiones, buffer, bytes + 1);
                procesar_peticiones_pendientes(&colaPeticiones, nuevo_socket, pIndice);
            }
            closesocket(nuevo_socket);
        }
        else
            ejecutando = 0;
    }
    closesocket(server_fd);
    VaciarCola(&colaPeticiones);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int GenerarRanking(const char *archPartidas, tArbolBin *pIndice, tLista *pRanking)
{
    FILE* pf;
    tPartida part;
    tRanking reg;
    tNodoArbolBin **nodo;
    tLista listaTemporal;
    int res = TODO_OK;

    pf = fopen(archPartidas, "rb");
    if(!pf)
    {
        return ERROR_ARCHIVO;
    }

    CrearLista(&listaTemporal);

    while(fread(&part, sizeof(tPartida), 1, pf) == 1)
    {
        nodo = BuscarPorIdEnArbol(pIndice, part.idjugador);
        if(nodo && *nodo)
        {
            strcpy(reg.Nombre, ((tIndiceJugador*)(*nodo)->Info)->nombre);
            reg.idJugador = part.idjugador;
            reg.TotalPuntos = part.Puntuacion;
            reg.partidas = 1;

            res = InsertarEnOrdenLista(&listaTemporal,
                                       &reg,
                                       sizeof(tRanking),
                                       SIN_DUPLICADOS,
                                       cmpRankingPorId,
                                       AcumularRanking);
            if(res != TODO_OK && res != DUPLICADO)
            {
                break;
            }
            res = TODO_OK;

        }
    }
    fclose(pf);
    if(res == TODO_OK)
    {

        while(listaTemporal)
        {
            sacarPrimeroLista(&listaTemporal, &reg, sizeof(tRanking));

            InsertarEnOrdenLista(pRanking,
                                 &reg,
                                 sizeof(tRanking),
                                 CON_DUPLICADOS,
                                 cmpRankingPorPuntosDesc,
                                 NULL);
        }
    }
    return res;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int altaJugadorServidor(const char *archIndice, tArbolBin *pIndice, const char *nombre, tCmp cmp)
{
    FILE* pf;
    tJugador nuevo;
    tIndiceJugador ind;

    pf = fopen(ARCH_JUGADORES, "ab");
    if(!pf)
    {
        return ERROR_ARCHIVO;
    }

    fseek(pf, 0, SEEK_END);
    nuevo.id = (int)(ftell(pf) / sizeof(tJugador)) + 1;

    memset(nuevo.nombre, 0, sizeof(nuevo.nombre));
    strncpy(nuevo.nombre, nombre, TAM_NOMBRE - 1);

    if(fwrite(&nuevo, sizeof(tJugador), 1, pf) != 1)
    {
        fclose(pf);
        return ERROR_ARCHIVO;
    }

    fclose(pf);

    ind.id = nuevo.id;
    ind.desplazamiento = nuevo.id - 1;
    strcpy(ind.nombre, nuevo.nombre);

    if(InsertarEnArbolBin(pIndice, &ind, sizeof(tIndiceJugador), cmp) != TODO_OK)
            return ERROR_REGISTRO;

    GuardarIndiceBinario(*pIndice, archIndice);

    return TODO_OK;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int registrarPartidaEnServidor(const char* arch, int id, float pts, int movs)
{
    FILE* pf = fopen(arch, "ab");
    if (!pf)
        return ERROR_ARCHIVO;

    tPartida p;
    p.idjugador = id;
    p.Puntuacion = pts;
    p.CantMovimientos = movs;

    fwrite(&p, sizeof(tPartida), 1, pf);
    fclose(pf);
    return TODO_OK;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void EscribirABBEnArchivo(tNodoArbolBin* raiz, FILE* arch)
{
    if(raiz == NULL)
        return;

    fwrite(raiz->Info, sizeof(tIndiceJugador), 1, arch);
    EscribirABBEnArchivo(raiz->Izq, arch);
    EscribirABBEnArchivo(raiz->Der, arch);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
tNodoArbolBin **BuscarPorIdEnArbol(tArbolBin* p, int id)
{
    tNodoArbolBin **izq;
    int idActual;
    if(!*p)
        return NULL;

    idActual = ((tIndiceJugador*)(*p)->Info)->id;
    if(idActual == id)
        return p;

    izq = BuscarPorIdEnArbol(&(*p)->Izq, id);
    if(izq)
        return izq;

    return BuscarPorIdEnArbol(&(*p)->Der, id);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GuardarIndiceBinario(tArbolBin raiz, const char *nombreArchivo)
{
    FILE* arch;

    arch = fopen(nombreArchivo, "wb");
    if(!arch)
        return;

    EscribirABBEnArchivo(raiz, arch);
    fclose(arch);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CargarIndiceBinario(tArbolBin* pIndice, const char *nombreArchivo)
{
    tIndiceJugador reg;
    FILE* arch = fopen(nombreArchivo, "rb");
    if(!arch)
    {
        printf("[SERVIDOR] No se encontro archivo de indice. Iniciando limpio.\n");
        return;
    }
    VaciarArbolBin(pIndice);
    fread(&reg, sizeof(tIndiceJugador), 1, arch);
    while(!feof(arch))
    {
        InsertarEnArbolBin(pIndice, &reg, sizeof(tIndiceJugador), cmpIndicePorId);
        fread(&reg, sizeof(tIndiceJugador), 1, arch);
    }

    fclose(arch);
    printf("[SERVIDOR] Indice cargado exitosamente desde %s\n", nombreArchivo);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CargarIndiceJugadores(const char *ArchJugadores, tArbolBin *pIndice)
{
    tJugador jugador;
    tIndiceJugador ind;
    int NumReg = 0;

    FILE *pJugadores = fopen(ArchJugadores, "rb");
    if(!pJugadores)
        return ERROR_ARCHIVO;

    fread(&jugador, sizeof(tJugador), 1, pJugadores);
    while(!feof(pJugadores))
    {
        ind.id = jugador.id;
        ind.desplazamiento = NumReg;

        strncpy(ind.nombre, jugador.nombre, sizeof(ind.nombre) - 1);
        ind.nombre[sizeof(ind.nombre) - 1] = '\0';

        InsertarEnArbolBin(pIndice, &ind, sizeof(tIndiceJugador), cmpIndicePorId);

        NumReg++;
        fread(&jugador, sizeof(tJugador), 1, pJugadores);
    }

    fclose(pJugadores);
    return TODO_OK;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
tNodoArbolBin** BuscarPorNombreEnArbol(tArbolBin* p, const char* nombre)
{
    tNodoArbolBin** izq;
    tIndiceJugador* info;

    if(!p || !*p)
        return NULL;

    info = (tIndiceJugador*)(*p)->Info;
    if(strcmp(info->nombre, nombre) == 0)
        return p;

    izq = BuscarPorNombreEnArbol(&(*p)->Izq, nombre);
    if(izq)
        return izq;

    return BuscarPorNombreEnArbol(&(*p)->Der, nombre);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AcumularRanking(void *nodoInfo, const void *elem)
{
    tRanking* existente = (tRanking*)nodoInfo;
    const tRanking *nuevo = (const tRanking*)elem;

    existente->TotalPuntos += nuevo->TotalPuntos;
    existente->partidas += 1;
}
///CMP///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int cmpIndicePorId(const void *a, const void *b)
{
    const tIndiceJugador *ia = (const tIndiceJugador*)a;
    const tIndiceJugador *ib = (const tIndiceJugador*)b;

    return ia->id - ib->id;
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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int cmpIndicePorNombre(const void *a, const void *b)
{
    const tIndiceJugador *ia = (const tIndiceJugador*)a;
    const tIndiceJugador *ib = (const tIndiceJugador*)b;

    return strcmp(ia->nombre, ib->nombre);
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
