#include "Servidor.h"
#include <time.h>

int main()
{
    tArbolBin indiceJugadores;

    printf("=== INICIALIZANDO SERVIDOR DEL VOLCAN ===\n");

    CrearArbolBin(&indiceJugadores);
    srand((unsigned int)time(NULL));

    CargarIndiceBinario(&indiceJugadores, ARCH_INDICE);

    if (iniciar_entorno_socket() != 0)
    {
        fprintf(stderr, "[ERROR] No se pudo inicializar WinSock.\n");
        VaciarArbolBin(&indiceJugadores);
        return -1;
    }
    ejecutar_servidor(PUERTO_DB, &indiceJugadores);

    GuardarIndiceBinario(indiceJugadores, ARCH_INDICE);

    printf("Cerrando servidor y liberando memoria...\n");
    VaciarArbolBin(&indiceJugadores);
    WSACleanup();
    printf("=== SERVIDOR APAGADO CORRECTAMENTE ===\n");
    return 0;
}
