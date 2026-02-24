#include "Servidor.h"
#include <time.h>

int main()
{
    tArbolBin indiceJugadores;
    int puerto = 8080;

    printf("=== INICIALIZANDO SERVIDOR DEL VOLCAN ===\n");

    CrearArbolBin(&indiceJugadores);
    srand((unsigned int)time(NULL));

    CargarIndiceBinario(&indiceJugadores, ARCH_INDICE);

    if (iniciar_entorno_socket() != 0)
    {
        printf("Error al iniciar WinSock.\n");
        VaciarArbolBin(&indiceJugadores);
        return -1;
    }
    ejecutar_servidor(puerto, &indiceJugadores);

    GuardarIndiceBinario(indiceJugadores, ARCH_INDICE);

    printf("Cerrando servidor y liberando memoria...\n");
    VaciarArbolBin(&indiceJugadores);
    WSACleanup();

    return 0;
}
