#include "Juego.h"
#include "Criatura.h"
#include <ctype.h>
#include <conio.h>
#include <mmsystem.h>

void IniciarMusicaFondo()
{
    PlaySound(TEXT(MUSICA_FONDO), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}
void DetenerMusica()
{
    PlaySound(NULL, 0, 0);
}
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

    if(destino)
    {
        estado->PosJugador = destino;
        info = (tInfoCamara*)destino->info;
        if (info->hay_lava)
        {
            estado->Vidas = 0;
        }
        if (info->hay_premio)
        {
            estado->Puntaje += 1;
            info->hay_premio = 0;
            Beep(800, 100);
        }
        if (info->hay_vida)
        {
            estado->Vidas++;
            info->hay_vida = 0;
            Beep(1000, 150);
        }
    }
}

void MostrarMapaJerarquico(tNodoArbolNario* nodo, tEstado* estado, int nivel, int xCentro, int yRelativo)
{
    tInfoCamara* info;
    tLista hijos;
    tLista aux;
    int k, cantidadHijos, nuevaX, anchoAbanico, separacionH, saltoV;
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

void DibujarInterfaz(tEstado* estado, int nivelLavaActual, int alturaMaxima)
{
    tInfoCamara* infoActual = (tInfoCamara*)estado->PosJugador->info;
    tNodoArbolNario* padre = NULL;
    int iteradorLava, iteradorVidas;
    HANDLE manejadorConsola = GetStdHandle(STD_OUTPUT_HANDLE);

    if (estado->Vidas == 1)
    {
        SetConsoleTextAttribute(manejadorConsola, 12 | FOREGROUND_INTENSITY);
    }
    else
    {
        SetConsoleTextAttribute(manejadorConsola, 14);
    }

    printf("==========================================================\n");
    printf(" ||             EXPLORACION DEL VOLCAN                 ||\n");
    printf("==========================================================\n");
    printf("  VIDAS: ");
    SetConsoleTextAttribute(manejadorConsola, 10);

    for(iteradorVidas = 0; iteradorVidas < estado->Vidas; iteradorVidas++)
    {
        printf("<3 ");
    }

    SetConsoleTextAttribute(manejadorConsola, 7);
    printf(" |  PUNTOS: %04d  |  TURNO: %d\n", estado->Puntaje, estado->TurnosJugados);
    printf("  NIVEL DE LAVA: [");
    SetConsoleTextAttribute(manejadorConsola, 12);
    for(iteradorLava = 0; iteradorLava < alturaMaxima; iteradorLava++)
    {
        if(iteradorLava >= nivelLavaActual) printf("#");
        else printf(".");
    }
    SetConsoleTextAttribute(manejadorConsola, 7);
    printf("]\n");
    printf("----------------------------------------------------------\n");
    if (BuscarPadre(&(estado->MapaPadres), estado->PosJugador, &padre))
        printf("    [^] Subir a Camara ID: %d\n", ((tInfoCamara*)padre->info)->id);
    else
        printf("    [!] Estas en la Salida (S)\n");
    printf("    [O] Ubicacion Actual ID: %d\n", infoActual->id);
    printf("----------------------------------------------------------\n");
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void EjecutarCicloJuego(tEstado *estado, tConfig *config, const char *archConfig)
{
    char teclaPresionada;
    int juegoActivo;
    int filaBaseMapa;
    int filaFinalInterfaz;
    int nivelLavaActual;
    int idCamaraOrigen;
    int iteradorSacudida;
    int Abandono = 0;
    tInfoCamara* infoJugador;
    tCola colaTurno;
    tMovimientoLog registroMovimiento;

    CrearCola(&colaTurno);
    CrearLista(&(estado->Historial));

    PlaySound(TEXT(MUSICA_FONDO), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);

    juegoActivo = 1;
    nivelLavaActual = config->altura_maxima;

    while (juegoActivo && estado->Vidas > 0)
    {
        system("cls");
        ubicarCursor(0,0);
        printf("===========================================================================\n");
        printf(" JUGADOR: %-10s | VIDAS: %d | TURNOS: %d | NIVEL LAVA: %d \n",
                "Jugador", estado->Vidas, estado->TurnosJugados, nivelLavaActual);
        printf("===========================================================================\n");

        filaBaseMapa = 4;
        MostrarMapaJerarquico(estado->Volcan, estado, 0, 75, filaBaseMapa);

        filaFinalInterfaz = filaBaseMapa + (config->altura_maxima * 4) + 4;
        ubicarCursor(0, filaFinalInterfaz);
        printf("Movimiento (W: Subir | A/S/D/F: Caminos) o Q para salir: ");

        teclaPresionada = tolower(getch());

        if (teclaPresionada == 'q')
        {
            juegoActivo = 0;
            Abandono = 1;
        }
        else if (teclaPresionada == 'w' || teclaPresionada == 'a' ||
                 teclaPresionada == 's' || teclaPresionada == 'd' ||
                 teclaPresionada == 'f')
        {
            PonerEnCola(&colaTurno, &teclaPresionada, sizeof(char));

            while (!ColaVacia(&colaTurno))
            {
                SacarDeCola(&colaTurno, &teclaPresionada, sizeof(char));

                idCamaraOrigen = ((tInfoCamara*)estado->PosJugador->info)->id;

                ProcesarMovimientoJugador(estado, teclaPresionada);

                infoJugador = (tInfoCamara*)estado->PosJugador->info;
                if (infoJugador->hay_lava)
                {
                    estado->Vidas = 0;
                    juegoActivo = 0;
                    Beep(200, 400);
                    break;
                }
                registroMovimiento.idOrigen = idCamaraOrigen;
                registroMovimiento.idDestino = ((tInfoCamara*)estado->PosJugador->info)->id;
                sprintf(registroMovimiento.descripcion, "Turno %d: Desplazamiento efectuado.", estado->TurnosJugados + 1);

                PonerAlFinalEnLista(&(estado->Historial), &registroMovimiento, sizeof(tMovimientoLog));

                VerificarCombate(estado);

                if (estado->Vidas > 0)
                {
                    MoverCriaturas(estado->Volcan, estado);
                    VerificarCombate(estado);
                }

                if (nivelLavaActual >= 0 && (rand() % 100 < 25)) /// aca definimos que la probabilidad de lava sea del 25%
                {
                    for(iteradorSacudida = 0; iteradorSacudida < 5; iteradorSacudida++)
                    {
                        system("color 47");
                        Sleep(40);
                        system("color 07");
                        Sleep(40);
                    }
                    AvanzarLava(estado->Volcan, nivelLavaActual, 0);
                    nivelLavaActual--;
                    infoJugador = (tInfoCamara*)estado->PosJugador->info;
                    if (infoJugador->hay_lava)
                    {
                        estado->Vidas = 0;
                        juegoActivo = 0;
                        Beep(400, 600);
                        printf("\n[!] LA LAVA TE ALCANZO. PARTIDA TERMINADA.\n");
                        Sleep(1500);
                    }
                }
            }
            if(juegoActivo)
            {
                estado->TurnosJugados++;
                infoJugador = (tInfoCamara*)estado->PosJugador->info;

                if (infoJugador->es_salida && estado->TurnosJugados > 1)
                {
                    PlaySound(NULL, 0, 0);
                    system("cls");
                    printf("\n¡HAS ESCAPADO! Puntos por vidas: %d\n", estado->Vidas * 5);
                    estado->Puntaje += (estado->Vidas * 5);
                    getch();
                    juegoActivo = 0;
                }
            }
        }
    }
    PlaySound(NULL, 0, 0);
    if(!Abandono)
    {
        system("cls");
        printf("\nGAME OVER - El volcan te ha consumido.\n");
        MostrarRegistroMovimientos(estado);
        getch();
    }

    VaciarLista(&(estado->Historial));
    VaciarCola(&colaTurno);
}
void MostrarRegistroMovimientos(tEstado* estado)
{
    tLista nodoActual;
    tMovimientoLog* datosRegistro;
    int numeroPaso;

    nodoActual = estado->Historial;
    numeroPaso = 1;

    printf("======================================================================\n");
    printf("                      REGISTRO DE LA EXPLORACION                      \n");
    printf("======================================================================\n");

    while(nodoActual)
    {
        datosRegistro = (tMovimientoLog*)nodoActual->Info;

        printf("[%02d] Camara %d ---> Camara %d | %s\n",
               numeroPaso++,
               datosRegistro->idOrigen,
               datosRegistro->idDestino,
               datosRegistro->descripcion);

        nodoActual = nodoActual->sig;
    }

    printf("======================================================================\n");
    printf("Presione una tecla para continuar...");
    getch();
}
