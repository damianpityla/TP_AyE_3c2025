#include "../Volcan/Volcan.h"

int main()
{
    tArbolBin indice;
    tLista ranking;
    int estado;

    CrearArbolBin(&indice);
    CrearLista(&ranking);

    estado = CargarIndiceJugadores(ARCH_JUGADORES, &indice); ///todavia no hay
    if(estado != TODO_OK)
    {
        puts("Error al cargar el archivo de indices.");
        VaciarLista(&ranking);
        VaciarArbolBin(&indice);
        return 1;
    }

    estado = GenerarRanking(ARCH_PARTIDAS, &indice, &ranking);
    if(estado != TODO_OK)
    {
        puts("Error al generar el ranking.");
        VaciarLista(&ranking);
        VaciarArbolBin(&indice);
        return 1;
    }
    puts("\n===== RANKING DE JUGADORES =====");
    RecorrerLista(&ranking, MostrarRanking);

    VaciarLista(&ranking);
    VaciarArbolBin(&indice);

    return 0;
}
