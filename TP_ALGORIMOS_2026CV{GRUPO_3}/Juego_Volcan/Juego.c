#include "Juego.h"
#include "Criatura.h"
#include <ctype.h>
#include <conio.h>

void ubicarCursor(int x, int y)
{
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD dwPos;
    dwPos.X = x;
    dwPos.Y = y;
    SetConsoleCursorPosition(hCon, dwPos);
}

int obtenerFilaActual()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(hConsole, &csbi))
        return csbi.dwCursorPosition.Y;
    return 0;
}

void AvanzarLava(tNodoArbolNario* nodo, int nivelObjetivo, int nivelActual)
{
    tInfoCamara* info;
    tLista hijos;

    if (!nodo) return;

    info = (tInfoCamara*)nodo->info;

    if (nivelActual == nivelObjetivo)
    {
        info->hay_lava = 1;
        info->hay_premio = 0;
        info->hay_vida = 0;
        info->cant_criaturas = 0;
        return;
    }

    hijos = nodo->hijos;
    while (hijos)
    {
        tNodoArbolNario* h = *(tNodoArbolNario**)hijos->Info;
        AvanzarLava(h, nivelObjetivo, nivelActual + 1);
        hijos = hijos->sig;
    }
}

void ProcesarMovimientoJugador(tEstado* estado, char tecla)
{
    tNodoArbolNario* destino = NULL;
    tInfoCamara* info;
    tLista hijos;
    int indiceHijo;
    int i;

    if (tecla == 'w')
    {
        BuscarPadre(&(estado->MapaPadres), estado->PosJugador, &destino);
    }
    else
    {
        indiceHijo = (tecla == 'a') ? 0 : (tecla == 's') ? 1 : (tecla == 'd') ? 2 : (tecla == 'f') ? 3 : -1;
        if (indiceHijo != -1)
        {
            hijos = estado->PosJugador->hijos;
            for (i = 0; i < indiceHijo && hijos; i++) hijos = hijos->sig;
            if (hijos) destino = *(tNodoArbolNario**)hijos->Info;
        }
    }

    if (destino)
    {
        estado->PosJugador = destino;
        info = (tInfoCamara*)destino->info;
        if (info->hay_lava) { estado->Vidas = 0; }
        if (info->hay_premio) { estado->Puntaje += 1; info->hay_premio = 0; }
        if (info->hay_vida) { estado->Vidas++; info->hay_vida = 0; }
    }
}

void MostrarMapaJerarquico(tNodoArbolNario* nodo, const tEstado* estado, int nivel, int xCentro, int yRelativo)
{
    tInfoCamara* info;
    tLista hijos;
    tLista aux;
    int i, k, cantidadHijos, nuevaX, anchoAbanico, separacionH, saltoV;
    HANDLE hConsole;

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    saltoV = 4;
    separacionH = (nivel == 0) ? 42 : (nivel == 1) ? 20 : (nivel == 2) ? 10 : 5;

    if (!nodo) return;

    info = (tInfoCamara*)nodo->info;
    ubicarCursor(xCentro, yRelativo);

    if (nivel == 0) { SetConsoleTextAttribute(hConsole, 12); printf("S"); }
    else { SetConsoleTextAttribute(hConsole, 14); printf("O"); }

    if (nodo == estado->PosJugador) { SetConsoleTextAttribute(hConsole, 11); printf("[J]"); }

    if (info->cant_criaturas > 0 || info->hay_premio || info->hay_lava || info->hay_vida)
    {
        SetConsoleTextAttribute(hConsole, 15);
        printf("[");
        if (info->cant_criaturas > 0) { SetConsoleTextAttribute(hConsole, 12); printf("C"); }
        if (info->hay_premio) { SetConsoleTextAttribute(hConsole, 14); printf("P"); }
        if (info->hay_vida) { SetConsoleTextAttribute(hConsole, 10); printf("V"); }
        if (info->hay_lava) { SetConsoleTextAttribute(hConsole, 12); printf("L"); }
        SetConsoleTextAttribute(hConsole, 15);
        printf("]");
    }

    hijos = nodo->hijos;
    if (hijos)
    {
        cantidadHijos = 0;
        aux = hijos;
        while (aux) { cantidadHijos++; aux = aux->sig; }
        anchoAbanico = (cantidadHijos - 1) * separacionH;
        nuevaX = xCentro - (anchoAbanico / 2);
        SetConsoleTextAttribute(hConsole, 8);
        ubicarCursor(xCentro, yRelativo + 1);
        printf("|");
        if (cantidadHijos > 1) {
            ubicarCursor(nuevaX, yRelativo + 2);
            for(k = 0; k <= anchoAbanico; k++) printf("-");
            for(k = 0; k < cantidadHijos; k++) {
                ubicarCursor(nuevaX + (k * separacionH), yRelativo + 2);
                printf("+");
                ubicarCursor(nuevaX + (k * separacionH), yRelativo + 3);
                printf("|");
            }
        } else {
            ubicarCursor(xCentro, yRelativo + 2); printf("|");
            ubicarCursor(xCentro, yRelativo + 3); printf("|");
        }
        while (hijos) {
            tNodoArbolNario* h = *(tNodoArbolNario**)hijos->Info;
            MostrarMapaJerarquico(h, estado, nivel + 1, nuevaX, yRelativo + saltoV);
            nuevaX += separacionH;
            hijos = hijos->sig;
        }
    }
    SetConsoleTextAttribute(hConsole, 7);
}

void DibujarInterfaz(const tEstado* estado)
{
    tInfoCamara* infoActual = (tInfoCamara*)estado->PosJugador->info;
    tNodoArbolNario* padre = NULL;
    tLista hijos = estado->PosJugador->hijos;
    int i = 0;
    char teclas[] = {'A', 'S', 'D', 'F'};
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    printf("==========================================================\n");
    printf(" ||             EXPLORACION DEL VOLCAN                 ||\n");
    printf("==========================================================\n");
    printf("  VIDAS: ");
    SetConsoleTextAttribute(hConsole, 10);
    for(int v=0; v < estado->Vidas; v++) printf("<3 ");
    SetConsoleTextAttribute(hConsole, 7);
    printf(" |  PUNTOS: %04d  |  TURNO: %d\n", estado->Puntaje, estado->TurnosJugados);
    printf("----------------------------------------------------------\n");
    if (BuscarPadre(&(estado->MapaPadres), estado->PosJugador, &padre))
        printf("    [^] Subir a Camara ID: %d\n", ((tInfoCamara*)padre->info)->id);
    else
        printf("    [!] Estas en la Salida (S)\n");
    printf("    [O] Ubicacion Actual ID: %d\n", infoActual->id);
    printf("----------------------------------------------------------\n");
}

void EjecutarCicloJuego(tEstado* estado, tConfig* config, const char* archConfig)
{
    char tecla;
    int juegoActivo;
    int filaBase;
    int filaFinal;
    int nivelLavaActual;
    tInfoCamara* infoJugador;

    juegoActivo = 1;
    nivelLavaActual = config->altura_maxima;

    while (juegoActivo && estado->Vidas > 0)
    {
        system("cls");
        DibujarInterfaz(estado);
        printf("\nMAPA DE EXPLORACION:\n");
        filaBase = obtenerFilaActual() + 1;
        MostrarMapaJerarquico(estado->Volcan, estado, 0, 75, filaBase);

        filaFinal = filaBase + (config->altura_maxima * 4) + 4;
        ubicarCursor(0, filaFinal);
        printf("Movimiento (W/A/S/D/F) o Q para salir: ");

        tecla = tolower(getch());

        if (tecla == 'q') juegoActivo = 0;
        else
        {
            ProcesarMovimientoJugador(estado, tecla);
            VerificarCombate(estado);

            if (estado->Vidas > 0)
            {
                MoverCriaturas(estado->Volcan, estado);
                VerificarCombate(estado);
            }

            if (estado->TurnosJugados > 0 && estado->TurnosJugados % 4 == 0 && nivelLavaActual >= 0)
            {
                AvanzarLava(estado->Volcan, nivelLavaActual, 0);
                nivelLavaActual--;
            }

            estado->TurnosJugados++;
            infoJugador = (tInfoCamara*)estado->PosJugador->info;

            if (infoJugador->es_salida && estado->TurnosJugados > 1)
            {
                system("cls");
                printf("\n¡HAS ESCAPADO! Puntos por vidas: %d\n", estado->Vidas * 5);
                estado->Puntaje += (estado->Vidas * 5);
                getch();
                juegoActivo = 0;
            }
        }
    }

    if (estado->Vidas <= 0)
    {
        system("cls");
        printf("\nGAME OVER - Te has quedado sin vidas.\n");
        getch();
    }
}
