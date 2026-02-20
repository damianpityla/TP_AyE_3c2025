#include "Servidor.h"
#include <time.h>

int main()
{
    tArbolBin indiceJugadores;
    int puerto = 8080;

    printf("=== INICIALIZANDO SERVIDOR DEL VOLCAN ===\n");

    CrearArbolBin(&indiceJugadores);
    srand((unsigned int)time(NULL));

    printf("Cargando indice de jugadores desde %s...\n", ARCH_JUGADORES);

    if (CargarIndiceJugadores(ARCH_JUGADORES, &indiceJugadores) == TODO_OK) {
        printf("Indice cargado correctamente.\n");
    } else {
        printf("No se encontro archivo de jugadores. Se iniciara vacio.\n");
    }

    if (iniciar_entorno_socket() != 0) {
        printf("Error al iniciar WinSock.\n");
        return -1;
    }

    ejecutar_servidor(puerto, &indiceJugadores);

    VaciarArbolBin(&indiceJugadores);
    WSACleanup();

    return 0;
}
