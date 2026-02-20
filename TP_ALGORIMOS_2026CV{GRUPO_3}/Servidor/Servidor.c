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
    sprintf(buffer, "PARTIDA %d %.2f %d", id_jugador, puntos, movimientos);

    send(sock, buffer, strlen(buffer), 0);

    // Recibir confirmación
    int bytes = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if(bytes > 0) {
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
    tArbolBin *nodo;

    while(SacarDeCola(pCola, peticion, sizeof(peticion)) == TODO_OK)
    {
        if (strncmp(peticion, "LOGIN", 5) == 0)
        {
            char nombre[50];
            sscanf(peticion + 6, "%s", nombre);
            tIndiceJugador clave;
            strcpy(clave.nombre, nombre);
            nodo = BusquedaEnArbolBin(pIndice, &clave, cmpIndicePorNombre);
            if (nodo)
            {
                sprintf(respuesta, "OK %d", ((tIndiceJugador*)(*nodo)->Info)->id);
            }
            else
            {
                sprintf(respuesta, "ERROR_USER_NOT_FOUND");
            }
            send(client_sock, respuesta, strlen(respuesta), 0);
        }
        else if (strncmp(peticion, "REGISTER", 8) == 0)
        {
            char nombre[50];
            sscanf(peticion + 9, "%s", nombre);
            tIndiceJugador clave;
            strcpy(clave.nombre, nombre);
            nodo = BusquedaEnArbolBin(pIndice, &clave, cmpIndicePorNombre);
            if (nodo)
            {
                sprintf(respuesta, "ERROR_ALREADY_EXISTS");
            }
            else
            {
                if (altaJugadorServidor(ARCH_JUGADORES, pIndice, nombre, cmpIndicePorNombre) == TODO_OK)
                {
                    sprintf(respuesta, "REGISTER_OK");
                }
                else
                {
                    sprintf(respuesta, "ERROR_DB");
                }
            }
            send(client_sock, respuesta, strlen(respuesta), 0);
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
            tLista ranking;
            tRanking reg;
            if (GenerarRanking(ARCH_PARTIDAS, pIndice, &ranking) == TODO_OK)
            {
                tLista actual = ranking;
                while(actual)
                {
                    memcpy(&reg, actual->Info, sizeof(tRanking));
                    send(client_sock, (char*)&reg, sizeof(tRanking), 0);
                    actual = actual->sig;
                }
            }
            reg.idJugador = -1;
            send(client_sock, (char*)&reg, sizeof(tRanking), 0);
            VaciarLista(&ranking);
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

    CrearCola(&colaPeticiones); // Requerimiento: peticiones encoladas

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET) return;

    direccion.sin_family = AF_INET;
    direccion.sin_addr.s_addr = INADDR_ANY;
    direccion.sin_port = htons(puerto);

    if (bind(server_fd, (struct sockaddr *)&direccion, sizeof(direccion)) == SOCKET_ERROR) return;

    listen(server_fd, 3);

    printf("Servidor de Base de Datos escuchando en puerto %d...\n", puerto);

    while(1)
    {
        nuevo_socket = accept(server_fd, (struct sockaddr *)&direccion, &addrlen);

        if (nuevo_socket != INVALID_SOCKET)
        {
            char buffer[1024] = {0};
            int bytes = recv(nuevo_socket, buffer, 1024, 0);

            if(bytes > 0)
            {
                // Encolamos el mensaje (Requerimiento h)
                PonerEnCola(&colaPeticiones, buffer, bytes + 1);

                // Pasamos los 3 argumentos: la cola, el socket y el índice ABB
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

int altaJugadorServidor(const char* arch, tArbolBin* pIndice, const char* nombre, tCmp cmp) {
    FILE* pf = fopen(arch, "ab+"); // ab+ para lectura/escritura al final
    if (!pf) return ERROR_ARCHIVO;

    tJugador nuevo;
    fseek(pf, 0, SEEK_END);
    nuevo.id = (ftell(pf) / sizeof(tJugador)) + 1;
    strncpy(nuevo.nombre, nombre, TAM_NOMBRE);
    nuevo.nombre[TAM_NOMBRE] = '\0';

    fwrite(&nuevo, sizeof(tJugador), 1, pf);

    tIndiceJugador ind;
    ind.id = nuevo.id;
    ind.desplazamiento = nuevo.id - 1;
    strcpy(ind.nombre, nuevo.nombre);

    InsertarEnArbolBin(pIndice, &ind, sizeof(tIndiceJugador), cmp);

    fclose(pf);
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
