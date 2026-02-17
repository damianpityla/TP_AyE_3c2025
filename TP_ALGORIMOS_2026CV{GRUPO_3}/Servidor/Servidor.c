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
        // 1. Caso LOGIN
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
                if (altaJugadorServidor(ARCH_JUGADORES, pIndice, nombre, cmpIndicePorNombre) == TODO_OK)
                {
                    nodo = BusquedaEnArbolBin(pIndice, &clave, cmpIndicePorNombre);
                    sprintf(respuesta, "OK %d", ((tIndiceJugador*)(*nodo)->Info)->id);
                }
                else
                {
                    sprintf(respuesta, "ERROR_ALTA");
                }
            }
            send(client_sock, respuesta, strlen(respuesta), 0);
        } // <--- Faltaba cerrar esta llave del bloque LOGIN

        // 2. Caso PARTIDA
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
