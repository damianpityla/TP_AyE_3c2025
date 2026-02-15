#include "Usuario.h"
#include "../Servidor/Servidor.h"
////********************************************************************************************************************************************************///
////********************************************************************************************************************************************************///
////********************************************************************************************************************************************************///
////********************************************************************************************************************************************************///

//-------------------MENU ONLINE-------------//

//-------------------MENU OFFLINE------------------------//

#define OPC_OFFLINE 2

int MenuOffline(void)
{
    const char* opciones[OPC_OFFLINE] = { "JUGAR", "SALIR" };
    int opcion = 0, salir = 0;
    int i, j;

    const int ANCHO = 90;                 // ancho interno del cuadro
    const char* MARGEN = "            ";  // margen izquierdo del cuadro

    unsigned char B = 254; // "cuadradito" (pixel)

    // Banner (5 líneas) hecho con caracteres, después reemplazamos '#' por B
    // (así queda prolijo y centrable)
    const char* banner[5] =
    {
        "  ###### #  # #### #### #####   ###   #### #      #    # ##### #     ###   ##### ##    #  ",
        "     #   #  # #    #    #   #   #  #  #    #      #    # #   # #    #   #  #   # # #   #  ",
        "     #   #  # #### # ## #   #   #   # ###  #      #    # #   # #    #      ##### #  #  #  ",
        "  #  #   #  # #    #  # #   #   #  #  #    #       #  #  #   # #    #   #  #   # #   # #  ",
        "  ####   #### #### #### #####   ###   #### ####     ##   ##### ####  ###   #   # #    ##  "
    };

    system("color 0C"); // rojo sobre negro

    while(!salir)
    {
        system("cls");

        printf("\n\n\n");

        // ┌──────────────────────────────┐
        printf("%s%c", MARGEN, 201);
        for(j = 0; j < ANCHO; j++) printf("%c", 205);
        printf("%c\n", 187);

        // 1 línea vacía
        printf("%s%c", MARGEN, 186);
        for(j = 0; j < ANCHO; j++) printf(" ");
        printf("%c\n", 186);

        // ====== IMPRIMIR BANNER CENTRADO ======
        for(i = 0; i < 5; i++)
        {
            int len = (int)strlen(banner[i]);
            int leftPad = (ANCHO - len) / 2; // centra dentro del cuadro

            printf("%s%c", MARGEN, 186);

            // espacios a la izquierda
            for(j = 0; j < leftPad; j++) printf(" ");

            // banner: cambiamos '#' por B
            for(j = 0; j < len; j++)
            {
                if(banner[i][j] == '#')
                    putchar(B);
                else
                    putchar(' ');
            }

            // completar el resto hasta ANCHO
            {
                int usados = leftPad + len;
                for(j = usados; j < ANCHO; j++) printf(" ");
            }

            printf("%c\n", 186);
        }

        // línea vacía
        printf("%s%c", MARGEN, 186);
        for(j = 0; j < ANCHO; j++) printf(" ");
        printf("%c\n", 186);

        // subtítulo centrado
        {
            const char* sub = "OFFLINE";
            int len = (int)strlen(sub);
            int leftPad = (ANCHO - len) / 2;

            printf("%s%c", MARGEN, 186);
            for(j = 0; j < leftPad; j++) printf(" ");
            printf("%s", sub);
            for(j = leftPad + len; j < ANCHO; j++) printf(" ");
            printf("%c\n", 186);
        }

        // 1 línea vacía
        printf("%s%c", MARGEN, 186);
        for(j = 0; j < ANCHO; j++) printf(" ");
        printf("%c\n", 186);

        // ====== opciones (centradas) ======
        for(i = 0; i < OPC_OFFLINE; i++)
        {
            char linea[64];

            if(i == opcion)
                snprintf(linea, sizeof(linea), "-> %s", opciones[i]);
            else
                snprintf(linea, sizeof(linea), "   %s", opciones[i]);

            {
                int len = (int)strlen(linea);
                int leftPad = (ANCHO - len) / 2;

                printf("%s%c", MARGEN, 186);
                for(j = 0; j < leftPad; j++) printf(" ");
                printf("%s", linea);
                for(j = leftPad + len; j < ANCHO; j++) printf(" ");
                printf("%c\n", 186);
            }
        }

        // espacio abajo (ajustá a gusto)
        for(i = 0; i < 8; i++)
        {
            printf("%s%c", MARGEN, 186);
            for(j = 0; j < ANCHO; j++) printf(" ");
            printf("%c\n", 186);
        }

        // └──────────────────────────────┘
        printf("%s%c", MARGEN, 200);
        for(j = 0; j < ANCHO; j++) printf("%c", 205);
        printf("%c\n", 188);

        // ====== controles ======
        {
            int tecla = getch();

// soporte flechas (tecla extendida)
            if (tecla == 0 || tecla == 224)
            {
                int ext = getch();
                if (ext == 72) tecla = 'w'; // ↑
                if (ext == 80) tecla = 's'; // ↓
            }

            if (tolower(tecla) == 'w')
            {
                opcion--;
                if (opcion < 0) opcion = OPC_OFFLINE - 1;
            }
            else if (tolower(tecla) == 's')
            {
                opcion++;
                if (opcion >= OPC_OFFLINE) opcion = 0;
            }
            else if (tecla == 13) // ENTER
            {
                if (opcion == 0)
                {
                    system("cls");
                    printf("\n\n\n\t\t\tElegiste JUGAR (offline)\n");
                    printf("\t\t\t(aca llamas a jugar())\n");
                    printf("\n\t\t\tPresione una tecla para volver...");
                    getch();
                }
                else
                {
                    system("cls");
                    printf("\n\n\n\t\t\tSaliendo...\n");
                    Sleep(500);
                    salir = 1;
                }
            }
        }
    }

    return 0;
}
////********************************************************************************************************************************************************///
////********************************************************************************************************************************************************///
////********************************************************************************************************************************************************///
////********************************************************************************************************************************************************///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int GenerarRanking(const char *ArchPartidas, tArbolBin *pIndice, tLista *pRanking)
{
    FILE *pPartidas;
    tPartida partida;
    tRanking reg;
    tIndiceJugador clave;
    tArbolBin *pNodo;

    CrearLista(pRanking);

    pPartidas = fopen(ArchPartidas, "rb");
    if(!pPartidas)
        return ERROR_ARCHIVO;

    fread(&partida, sizeof(tPartida), 1, pPartidas);
    while(!feof(pPartidas))
    {
        reg.idJugador   = partida.idjugador;
        reg.TotalPuntos = partida.Puntuacion;
        reg.partidas    = 1;

        clave.id = reg.idJugador;
        pNodo = BusquedaEnArbolBin(pIndice, &clave, cmpIndicePorId);

        if(pNodo)
            strcpy(reg.Nombre, ((tIndiceJugador*)(*pNodo)->Info)->nombre);
        else
            strcpy(reg.Nombre, "DESCONOCIDO");

        InsertarEnOrdenLista(pRanking, &reg, sizeof(tRanking), SIN_DUPLICADOS, cmpRankingPorId, AcumularRanking);

        fread(&partida, sizeof(tPartida), 1, pPartidas);
    }

    fclose(pPartidas);

    OrdenarLista(pRanking, cmpRankingPorPuntosDesc);

    return TODO_OK;
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
void AcumularRanking(void *nodoInfo, const void *elem)
{
    tRanking* existente = (tRanking*)nodoInfo;
    const tRanking *nuevo = (const tRanking*)elem;

    existente->TotalPuntos += nuevo->TotalPuntos;
    existente->partidas += 1;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MostrarRanking(const void *a)
{
    const tRanking* aux = (const tRanking*)a;
    printf("ID: %d  NOMBRE: %-20s  PUNTOS: %8.2f  PARTIDAS: %d\n",
           aux->idJugador,
           aux->Nombre,
           aux->TotalPuntos,
           aux->partidas);
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
    const tRanking *ra = (const tRanking*)a;
    const tRanking *rb = (const tRanking*)b;

    if(fabsf(ra->TotalPuntos - rb->TotalPuntos) < EPSILON) /// cmp entre flotantes le pifia
        return strcmp(ra->Nombre, rb->Nombre);

    if(ra->TotalPuntos < rb->TotalPuntos)
        return 1;
    else
        return -1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int cmpRankingPorId(const void *a, const void *b)
{
    const tRanking *ra = (const tRanking*)a;
    const tRanking *rb = (const tRanking*)b;

    return ra->idJugador - rb->idJugador;
}

