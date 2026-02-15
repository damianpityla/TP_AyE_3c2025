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
    char linea[TAMLINEA];
    int opcion = 0, salir = 0;
    int i, j,len,leftPad, tecla,ext;




    const int ANCHO = 100;
    const char* MARGEN = "          ";//Margen por fuera del Recuadro

    //siento que asi le da un estilo de Juego
    char titulo[TAM_TITULO][500] = {"  ###### #   # #### ##### #####    #####  ##### #      #      # ##### #      #####  ###### ##    #  ",
                                 "      #  #   # #    #     #   #    #    # #     #      #      # #   # #     #     # #    # # #   #  ",
                                 "      #  #   # ###  # ### #   #    #    # ###   #       #    #  #   # #     #       ###### #  #  #  ",
                                 "  #   #  #   # #    #   # #   #    #    # #     #        #  #   #   # #     #     # #    # #   # #  ",
                                 "  #####  ##### #### ### # #####    #####  ##### #####     ##    ##### #####  #####  #    # #    ##  ",
                                 "                        #                                                                           "
                               };

    system("color 04");//Este Color esta Bueno

    while(!salir)
    {
        system("cls");
        printf("\n");

        // ┌───┐ asi se hace un techo
        printf("%s%c", MARGEN, 201);
        for(j = 0; j < ANCHO; j++) printf("%c", 205);
        printf("%c\n", 187);

        // hacemos un salto de linea
        printf("%s%c", MARGEN, 186);
        for(j = 0; j < ANCHO; j++)
            printf(" ");
        printf("%c\n", 186);

        /// TITULO "JUEGO DEL VOLCAN"
        for(i = 0; i < TAM_TITULO; i++)
        {
            len = strlen(titulo[i]);
            leftPad = (ANCHO - len) / 2; // para centrar dentro del Recuadro

            printf("%s%c", MARGEN, 186);

            // espacios a la izquierda
            for(j = 0; j < leftPad; j++)
                printf(" ");

            for(j = 0; j < len; j++)
                printf("%c",titulo[i][j]);// Imprimimos el Titulo

            // completar el resto hasta ANCHO
            for(j = leftPad + len; j < ANCHO; j++)
                printf(" ");
            printf("%c\n", 186);
        }

        // línea vacía
        printf("%s%c", MARGEN, 186);
        for(j = 0; j < ANCHO; j++)
            printf(" ");
        printf("%c\n", 186);

        //ESTADO DEL MENU ->"ESTADO OFFLINE" = 14 CARACTERES
        leftPad = (ANCHO - TAM_ESTADO) / 2;

        printf("%s%c", MARGEN, 186);
        for(j = 0; j < leftPad ; j++)
            printf(" ");
        printf("ESTADO OFFLINE");
        for(j = leftPad + TAM_ESTADO; j < ANCHO; j++)
            printf(" ");
        printf("%c\n", 186);

        //Salto de Linea
        printf("%s%c", MARGEN, 186);
        for(j = 0; j < ANCHO; j++)
            printf(" ");
        printf("%c\n", 186);

        // ====== opciones (centradas) ======
        for(i = 0; i < OPC_OFFLINE; i++)
        {

            if(i == opcion)
                snprintf(linea, sizeof(linea), "-> %s", opciones[i]);
            else
                snprintf(linea, sizeof(linea), "   %s", opciones[i]);

            len = strlen(linea);
            leftPad = (ANCHO - len) / 2;

            printf("%s%c", MARGEN, 186);
            for(j = 0; j < leftPad; j++)
                printf(" ");

            printf("%s", linea);

            for(j = leftPad + len; j < ANCHO; j++)
                printf(" ");
            printf("%c\n", 186);
        }

        // espacio abajo (ajustá a gusto)
        printf("%s%c", MARGEN, 186);
        for(j = 0; j < ANCHO; j++) printf(" ");
        printf("%c\n", 186);

        // └──────────────────────────────┘
        printf("%s%c", MARGEN, 200);
        for(j = 0; j < ANCHO; j++) printf("%c", 205);
        printf("%c\n", 188);

        ///CONTROLES IMPORTANTISIMO
        tecla = getch();
        if (tecla == 0 || tecla == 224)
        {
            ext = getch();
            if (ext == 72)
                tecla = 'w';
            if (ext == 80)
                tecla = 's';
        }

        if (tolower(tecla) == 'w')
        {
            opcion--;
            if(opcion < 0)
                opcion = OPC_OFFLINE - 1;
        }
        else if (tolower(tecla) == 's')
        {
            opcion++;
            if(opcion >= OPC_OFFLINE)
                opcion = 0;
        }
        else if (tecla == 13) // toco Enter
        {
            if (opcion == 0)
            {
                ///LOGICA DEL JUEGO QUE DEBE ANDAR TANTO PARA ONLINE COMO OFFLINE///
                system("cls");
                printf("\n\n\n\t\t\tElegiste JUGAR (offline)\n");
                printf("\t\t\t(aca llamas a jugar())\n");
                printf("\n\t\t\tPresione una tecla para volver...");
                getch();
            }
            else
            {
                ///SALIS DEL WHILE Y POR LO TANTO SALIS DEL MENU Y BUENO TERMINA EL JUEGO///
                system("cls");
                printf("\n\n\n\t\t\tSaliendo...\n");
                Sleep(1000);
                salir = 1;
            }
        }

    }

    return TODO_OK;
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

