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

    CrearArbolBin(&indice);
    CrearLista(&ranking);

    srand(time(NULL));

    if(!Menu())
    {
        VaciarLista(&ranking);
        VaciarArbolBin(&indice);
        return 0;
    }

    VaciarLista(&ranking);
    VaciarArbolBin(&indice);

    puts("\tMuchas gracias por jugar! esperamos vuelva pronto.");
    puts("Presione ENTER para salir...");
    getchar();

    return 0;
}
