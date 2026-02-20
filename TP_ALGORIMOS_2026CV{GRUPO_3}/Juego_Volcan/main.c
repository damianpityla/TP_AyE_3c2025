#include "Volcan.h"
#include "Usuario.h"
#include "Juego.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    tArbolBin indice;
    tLista ranking;
    tConfig config;
    tEstado estado;
    int res;
    int modoOnline;

    CrearArbolBin(&indice);
    CrearLista(&ranking);
    srand(time(NULL));
    modoOnline = 0;

    if(!Menu())
    {
        VaciarLista(&ranking);
        VaciarArbolBin(&indice);
        return 0;
    }

    res = CargarIndiceJugadores(ARCH_JUGADORES, &indice);
    if(res == TODO_OK)
    {
        modoOnline = 1;
    }
    else
    {
        puts("Iniciando en modo OFFLINE (No se guardaran puntuaciones).");
    }

    if(!CargarConfiguracion(&config, "config.txt"))
    {
        puts("Error critico: No se encontro config.txt");
        VaciarLista(&ranking);
        VaciarArbolBin(&indice);
        return 1;
    }

    if(!GenerarEstructuraVolcan(&estado, &config))
    {
        puts("Error al generar el volcan.");
        VaciarLista(&ranking);
        VaciarArbolBin(&indice);
        return 1;
    }

    PoblarCamaras(&estado, &config);

    GrabarArchivoVolcan(estado.Volcan, &estado, "volcan.txt");

    EjecutarCicloJuego(&estado, &config, "config.txt");

    if(modoOnline)
    {
        res = GenerarRanking(ARCH_PARTIDAS, &indice, &ranking);
        if(res == TODO_OK)
        {
            puts("\n===== RANKING DE JUGADORES =====");
            RecorrerLista(&ranking, MostrarRanking);
        }
    }

    VaciarLista(&ranking);
    VaciarArbolBin(&indice);

    return 0;
}
