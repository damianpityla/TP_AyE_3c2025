#include "Volcan.h"

int main()
{
    tArbol indice;
    tLista ranking;
    int estado;

    CrearArbol(&indice);
    CrearLista(&ranking);

    estado = CargarIndiceJugadores(ARCH_JUGADORES, &indice); ///todavia no hay
    if(estado != TODO_OK)
    {
        puts("Error al cargar el archivo de indices.");
        VaciarLista(&ranking);
        VaciarArbol(&indice);
        return 1;
    }

    estado = GenerarRanking(ARCH_PARTIDAS, &indice, &ranking);
    if(estado != TODO_OK)
    {
        puts("Error al generar el ranking.");
        VaciarLista(&ranking);
        VaciarArbol(&indice);
        return 1;
    }
    puts("\n===== RANKING DE JUGADORES =====");
    RecorrerLista(&ranking, MostrarRanking);

    VaciarLista(&ranking);
    VaciarArbol(&indice);

    return 0;
}
