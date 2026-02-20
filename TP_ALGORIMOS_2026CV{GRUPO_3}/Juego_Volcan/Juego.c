#include "Juego.h"
#include "Criatura.h"
void EjecutarCicloJuego(tEstado* estado, struct sConfig* config, const char *ArchConfiguraciones)
{
    int jugando = 1;
    char tecla;
    tCola colaMovs;
    int* prefijoMapa = (int*)calloc(100, sizeof(int));
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    CrearCola(&colaMovs);
    CrearArchivoConfiguracion(ArchConfiguraciones);

    while(jugando && estado->Vidas > 0)
    {
        DibujarInterfaz(estado);

        if (prefijoMapa)
        {
            memset(prefijoMapa, 0, 100 * sizeof(int));
            printf("\nMAPA DE EXPLORACION:\n");
            MostrarMapaJerarquico(estado->Volcan, estado, 0, prefijoMapa, 1);
        }

        tecla = getch();

        if (tecla == 'q' || tecla == 'Q')
        {
            jugando = 0;
        }
        else
        {
            PonerEnCola(&colaMovs, &tecla, sizeof(char));
            ActualizarEstadoJuego(estado, &colaMovs, &jugando);

            MoverCriaturas(estado->Volcan, estado);

            tInfoCamara* info = (tInfoCamara*)estado->PosJugador->info;
            if (info->cant_criaturas > 0)
            {
                estado->Vidas--;
                SetConsoleTextAttribute(hConsole, 12); /* Rojo */
                printf("\n\n !!! ¡HAS SIDO HERIDO POR UNA CRIATURA! !!!\n");
                SetConsoleTextAttribute(hConsole, 7);
                printf("Presiona una tecla para continuar...");
                getch();

                if (estado->Vidas <= 0) jugando = 0;
            }

            GrabarArchivoVolcan(estado->Volcan, estado, ARCH_VOLCAN);
        }
    }

    if (prefijoMapa) free(prefijoMapa);
    VaciarCola(&colaMovs);
}
int ProcesarEntradaJugador(tCola* colaMovimientos)
{
    int tecla;
    tecla = tolower(getch());

    if (tecla == 'q')
        return 0;

    if (strchr("wasdf", tecla))
    {
        PonerEnCola(colaMovimientos, &tecla, sizeof(int));
        return 1;
    }

    return 0;
}

void ActualizarEstadoJuego(tEstado* estado, tCola* colaMovimientos, int* jugando)
{
    char mov;
    tNodoArbolNario* destino = NULL;
    tInfoCamara* info;

    if (SacarDeCola(colaMovimientos, &mov, sizeof(char)))
    {
        mov = toupper(mov);

        if (mov == 'W')
        {
            BuscarPadre(&(estado->MapaPadres), estado->PosJugador, &destino);
        }
        else if (strchr("ASDF", mov))
        {
            int pos = (mov == 'A') ? 0 : (mov == 'S') ? 1 : (mov == 'D') ? 2 : 3;
            HijoEnPos(estado->PosJugador, pos, &destino);
        }

        if (destino != NULL)
        {
            estado->PosJugador = destino;
            estado->TurnosJugados++;

            info = (tInfoCamara*)destino->info;

            if (info->hay_premio)
            {
                estado->Puntaje += 100;
                info->hay_premio = 0;
            }

            if (info->hay_vida)
            {
                estado->Vidas++;
                info->hay_vida = 0;
            }

            if (info->hay_lava)
            {
                estado->Vidas = 0;
                *jugando = 0;
            }

            if (info->es_salida && estado->TurnosJugados > 1)
            {
                *jugando = 0;
            }
        }

    }
}

void DibujarInterfaz(const tEstado* estado)
{
    tInfoCamara* infoActual = (tInfoCamara*)estado->PosJugador->info;
    tNodoArbolNario* padre = NULL;
    tLista hijos = estado->PosJugador->hijos;
    int i = 0;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    system("cls");
    printf("==========================================================\n");
    printf(" ||             EXPLORACION DEL VOLCAN                 ||\n");
    printf("==========================================================\n");

    printf("  VIDAS: ");
    SetConsoleTextAttribute(hConsole, 10);
    for(int v=0; v < estado->Vidas; v++) printf("<3 ");

    SetConsoleTextAttribute(hConsole, 7);
    printf(" |  PUNTOS: ");
    SetConsoleTextAttribute(hConsole, 14);
    printf("%04d", estado->Puntaje);

    SetConsoleTextAttribute(hConsole, 7);
    printf("  |  TURNO: %d\n", estado->TurnosJugados);
    printf("----------------------------------------------------------\n");

    printf("  [ NIVEL SUPERIOR ]\n");
    if (BuscarPadre(&(estado->MapaPadres), estado->PosJugador, &padre)) {
        printf("    [^] Salida hacia Camara ID: %d\n", ((tInfoCamara*)padre->info)->id);
    } else {
        printf("    [!] No hay salida superior (Estas en el Crater)\n");
    }

    printf("\n  [ TU UBICACION ]\n");
    printf("    [O] Estas en la Camara ID: %d\n", infoActual->id);

    printf("\n  [ TUNELES INFERIORES ]\n");
    if (hijos == NULL) {
        printf("    [-] Camino sin salida.\n");
    } else {
        char teclas[] = {'A', 'S', 'D', 'F'};
        while (hijos && i < 4) {
            tNodoArbolNario* h = *(tNodoArbolNario**)hijos->Info;
            printf("    [%c] Tunel hacia Camara ID: %d\n", *(teclas + i), ((tInfoCamara*)h->info)->id);
            hijos = hijos->sig;
            i++;
        }
    }
    printf("----------------------------------------------------------\n");
    printf("  Siguiente movimiento (W/A/S/D/F) o Q para salir: ");
}
void MostrarEntorno(tEstado* estado)
{
    tNodoArbolNario* padre = NULL;
    tLista hijos = estado->PosJugador->hijos;
    tInfoCamara* infoHijo;
    int i = 0;
    char teclas[] = {'A', 'S', 'D', 'F'};

    printf("\n--- RADAR DE PROXIMIDAD ---\n");

    // 1. Mirar hacia ARRIBA (Padre)
    if (BuscarPadre(&(estado->MapaPadres), estado->PosJugador, &padre)) {
        printf("  [W] Subir a Camara ID: %d\n", ((tInfoCamara*)padre->info)->id);
    } else {
        printf("  [W] Estas en la salida (Crater)\n");
    }

    // 2. Mirar hacia ABAJO (Hijos)
    if (hijos == NULL) {
        printf("  [-] No hay tuneles hacia abajo.\n");
    } else {
        printf("  Tuneles inferiores:\n");
        while (hijos && i < 4) {
            // Recuperamos el puntero al nodo hijo guardado en la lista
            tNodoArbolNario* h = *(tNodoArbolNario**)hijos->Info;
            infoHijo = (tInfoCamara*)h->info;

            printf("    [%c] Camara ID: %d", teclas[i], infoHijo->id);

            // Un pequeño "spoiler" o pista para el jugador
            if (infoHijo->hay_lava) printf(" (¡Calor extremo!)");
            if (infoHijo->cant_criaturas > 0) printf(" (Gruñidos...)");

            printf("\n");
            hijos = hijos->sig;
            i++;
        }
    }
    printf("---------------------------\n");
}
void MostrarMapaJerarquico(tNodoArbolNario* nodo, const tEstado* estado, int nivel, int* prefijo, int esUltimo)
{
    tInfoCamara* info;
    tLista hijos;
    int i;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    if(!nodo)
        return;

    info = (tInfoCamara*)nodo->info;

    if(nivel > 0)
    {
        for (i = 0; i < nivel - 1; i++)
        {
            SetConsoleTextAttribute(hConsole, 8);
            if (*(prefijo + i)) printf("|       ");
            else printf("        ");
        }
        printf("|\n");
    }

    for (i = 0; i < nivel - 1; i++)
    {
        if (*(prefijo + i)) printf("|       ");
        else printf("        ");
    }

    if (nivel > 0)
    {
        if (esUltimo)
        {
            printf("L--");
            *(prefijo + (nivel - 1)) = 0;
        }
        else
        {
            printf("|--");
            *(prefijo + (nivel - 1)) = 1;
        }
        SetConsoleTextAttribute(hConsole, 14);
        printf("(O)-- ");
    }
    else
    {
        SetConsoleTextAttribute(hConsole, 12);
        printf("( S ) CRATER");
    }

    if (nodo == estado->PosJugador)
    {
        SetConsoleTextAttribute(hConsole, 11);
        printf(" < JUGADOR >");
    }
    else if (nivel > 0)
    {
        if (info->cant_criaturas > 0 || info->hay_premio || info->hay_lava || info->hay_vida)
        {
            SetConsoleTextAttribute(hConsole, 15);
            printf(" <");
            if (info->cant_criaturas > 0) {
                SetConsoleTextAttribute(hConsole, 12);
                printf("C");
            }
            if (info->hay_premio) {
                SetConsoleTextAttribute(hConsole, 14);
                printf("P");
            }
            if (info->hay_vida) {
                SetConsoleTextAttribute(hConsole, 10);
                printf("V");
            }
            if (info->hay_lava) {
                SetConsoleTextAttribute(hConsole, 12);
                printf("L");
            }
            SetConsoleTextAttribute(hConsole, 15);
            printf(">");
        }
    }

    SetConsoleTextAttribute(hConsole, 8);
    printf(" [ID:%d]\n", info->id);
    SetConsoleTextAttribute(hConsole, 7);

    hijos = nodo->hijos;
    while (hijos)
    {
        tNodoArbolNario* h = *(tNodoArbolNario**)hijos->Info;
        int ultimo = (hijos->sig == NULL);
        *(prefijo + nivel) = !ultimo;
        MostrarMapaJerarquico(h, estado, nivel + 1, prefijo, ultimo);
        hijos = hijos->sig;
    }
}
