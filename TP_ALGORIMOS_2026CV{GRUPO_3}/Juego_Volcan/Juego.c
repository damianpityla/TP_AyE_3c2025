#include "Juego.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void IniciarMusicaFondo()
{
    PlaySound(TEXT(MUSICA_FONDO), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DetenerMusica()
{
    PlaySound(NULL, 0, 0);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ubicarCursor(int x, int y)
{
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD dwPos;
    dwPos.X = x;
    dwPos.Y = y;
    SetConsoleCursorPosition(hCon, dwPos);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int obtenerFilaActual()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(hConsole, &csbi))
        return csbi.dwCursorPosition.Y;
    return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AvanzarLava(tNodoArbolNario *nodo, int nivelObjetivo, int nivelActual)
{
    tInfoCamara *info;
    tLista hijos;

    if(!nodo)
        return;

    info = (tInfoCamara*)nodo->info;

    if(nivelActual == nivelObjetivo)
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ProcesarMovimientoJugador(tEstado* estado, char tecla)
{
    tNodoArbolNario *destino = NULL;
    tInfoCamara *info;
    tLista hijos;
    int indiceHijo;
    int i, cantHijos = 0;
    tLista aux;

    if(tecla == 'w')
    {
        BuscarPadre(&(estado->MapaPadres), estado->PosJugador, &destino);
    }
    else
    {
        switch(tecla)
        {
            case 'a': indiceHijo = 0;  break;
            case 's': indiceHijo = 1;  break;
            case 'd': indiceHijo = 2;  break;
            case 'f': indiceHijo = 3;  break;
            default:  indiceHijo = -1; break;
        }
        aux = estado->PosJugador->hijos;
        while(aux)
        {
            cantHijos++;
            aux = aux->sig;
        }

        if(indiceHijo != -1 && cantHijos > 0)
        {
            hijos = estado->PosJugador->hijos;
            if(cantHijos == 1)
            {
                destino = *(tNodoArbolNario**)hijos->Info;
            }
            else
            {
                for (i = 0; i < indiceHijo && hijos; i++)
                    hijos = hijos->sig;

                if(hijos)
                    destino = *(tNodoArbolNario**)hijos->Info;
            }
        }
    }

    if(destino)
    {
        estado->PosJugador = destino;
        info = (tInfoCamara*)destino->info;

        if (info->hay_premio)
        {
            estado->Puntaje += 10;
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

    if(!nodo)
        return;

    info = (tInfoCamara*)nodo->info;
    ubicarCursor(xCentro, yRelativo);

    if (nivel == 0) { SetConsoleTextAttribute(hConsole, 12); printf("S"); }
    else { SetConsoleTextAttribute(hConsole, 14); printf("O"); }

    if (nodo == estado->PosJugador) { SetConsoleTextAttribute(hConsole, 11); printf("[J]"); }

    if (info->cant_criaturas > 0 || info->hay_premio || info->hay_lava || info->hay_vida)
    {
        SetConsoleTextAttribute(hConsole, 15);
        printf("[");
        if (info->cant_criaturas > 0)
        {
            SetConsoleTextAttribute(hConsole, 12);
            printf("C");
        }
        if (info->hay_premio)
        {
            SetConsoleTextAttribute(hConsole, 14);
            printf("P");
        }
        if (info->hay_vida)
        {
            SetConsoleTextAttribute(hConsole, 10);
            printf("V");
        }
        if (info->hay_lava)
        {
            SetConsoleTextAttribute(hConsole, 12);
            printf("L");
        }
        SetConsoleTextAttribute(hConsole, 15);
        printf("]");
    }

    hijos = nodo->hijos;
    if(hijos)
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
            for(k = 0; k <= anchoAbanico; k++)
                printf("-");
            for(k = 0; k < cantidadHijos; k++)
            {
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DibujarInterfaz(tEstado *estado, int nivelLavaActual, int alturaMaxima)
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
    int bonusVidas;
    int i;
    tInfoCamara *infoJugador;
    tCola colaTurno;
    tCola colaEventosPC;
    tMovimientoLog registroMovimiento;
    tEventoComputadora evento;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    CrearCola(&colaEventosPC);
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
                /// encolado de IA de Criaturas
                evento.tipo = MOV_CRIATURA;
                PonerEnCola(&colaEventosPC, &evento, sizeof(tEventoComputadora));

                /// encolado de Avance de Lava
                if (nivelLavaActual >= 0 && (rand() % 100 < 25))
                {
                    evento.tipo = MOV_LAVA;
                    evento.nivelLava = nivelLavaActual;
                    PonerEnCola(&colaEventosPC, &evento, sizeof(tEventoComputadora));
                }

                registroMovimiento.idOrigen = idCamaraOrigen;
                registroMovimiento.idDestino = ((tInfoCamara*)estado->PosJugador->info)->id;
                sprintf(registroMovimiento.descripcion, "Turno %d: Desplazamiento efectuado.", estado->TurnosJugados + 1);

                PonerAlFinalEnLista(&(estado->Historial), &registroMovimiento, sizeof(tMovimientoLog));
            }

            /// desencolado de Eventos de la Computadora
            while(!ColaVacia(&colaEventosPC) && juegoActivo)
            {
                SacarDeCola(&colaEventosPC, &evento, sizeof(tEventoComputadora));

                if (evento.tipo == MOV_CRIATURA)
                {
                    MoverCriaturas(estado->Volcan, estado);
                    VerificarCombate(estado);
                }
                else if (evento.tipo == MOV_LAVA)
                {
                    for(iteradorSacudida = 0; iteradorSacudida < 5; iteradorSacudida++)
                    {
                        system("color 47");
                        Sleep(40);
                        system("color 07");
                        Sleep(40);
                    }
                    AvanzarLava(estado->Volcan, evento.nivelLava, 0);
                    nivelLavaActual--;
                }
                infoJugador = (tInfoCamara*)estado->PosJugador->info;
                if (infoJugador->hay_lava || estado->Vidas <= 0)
                {
                    if(infoJugador->hay_lava) estado->Vidas = 0;
                    juegoActivo = 0;
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
                    bonusVidas = estado->Vidas;

                    Beep(523, 150);
                    Beep(659, 150);
                    Beep(783, 150);
                    Beep(1046, 300);

                    SetConsoleTextAttribute(hConsole, CLR_AMARILLO);
                    printf("\n\t%c", 201);
                    for(i = 0; i < 50; i++) printf("%c", 205);
                    printf("%c\n", 187);

                    printf("\t%c                VICOTRIA ALCANZADA                %c\n", 186, 186);
                    printf("\t%c         ¡HAS LOGRADO ESCAPAR DEL VOLCAN!         %c\n", 186, 186);

                    printf("\t%c", 204);
                    for(i = 0; i < 50; i++) printf("%c", 205);
                    printf("%c\n", 185);

                    SetConsoleTextAttribute(hConsole, 15);
                    printf("\t%c  Resumen de la mision:                           %c\n", 186, 186);
                    printf("\t%c    - Puntos acumulados: %10d               %c\n", 186, estado->Puntaje, 186);
                    printf("\t%c    - Vidas restantes:   %10d               %c\n", 186, estado->Vidas, 186);

                    SetConsoleTextAttribute(hConsole, 10);
                    printf("\t%c    - Bono por supervivencia : %4d               %c\n", 186, bonusVidas, 186);

                    SetConsoleTextAttribute(hConsole, CLR_AMARILLO);
                    printf("\t%c", 200);
                    for(i = 0; i < 50; i++) printf("%c", 205);
                    printf("%c\n", 188);

                    estado->Puntaje += bonusVidas;

                    printf("\n\t  TOTAL FINAL: %d PUNTOS", estado->Puntaje);
                    printf("\n\n\t>> Presione una tecla para continuar...");
                    getch();
                    juegoActivo = 0;
                }
            }
        }
    }

    PlaySound(NULL, 0, 0);
    system("cls");

    if(Abandono)
    {
        SetConsoleTextAttribute(hConsole, CLR_CYAN);
        printf("\n\t%c", 201);
        for(i = 0; i < 50; i++) printf("%c", 205);
        printf("%c\n", 187);
        printf("\t%c                MISION ABORTADA                   %c\n", 186, 186);
        printf("\t%c         Has decidido retirarte del volcan        %c\n", 186, 186);
        printf("\t%c", 200);
        for(i = 0; i < 50; i++) printf("%c", 205);
        printf("%c\n", 188);

        SetConsoleTextAttribute(hConsole, 7);
        printf("\n\tPuntaje final acumulado: %d puntos.\n", estado->Puntaje);
        printf("\n\t>> Presione una tecla para ver la bitacora...");
        getch();
        MostrarRegistroMovimientos(estado);
        getch();
    }
    else if (estado->Vidas <= 0)
    {
        Beep(330, 300);
        Beep(131, 500);
        SetConsoleTextAttribute(hConsole, CLR_ROJO);
        printf("\n\t%c", 201);
        for(i = 0; i < 50; i++) printf("%c", 205);
        printf("%c\n", 187);
        printf("\t%c                   GAME OVER                      %c\n", 186, 186);
        printf("\t%c        El volcan ha reclamado tu alma...         %c\n", 186, 186);
        printf("\t%c", 200);
        for(i = 0; i < 50; i++) printf("%c", 205);
        printf("%c\n", 188);

        SetConsoleTextAttribute(hConsole, 7);
        printf("\n\tPuntaje final acumulado: %d puntos.\n", estado->Puntaje);
        printf("\n\t>> Presione una tecla para ver la bitacora...");
        getch();
        MostrarRegistroMovimientos(estado);
        getch();
    }

    VaciarLista(&(estado->Historial));
    VaciarCola(&colaTurno);
    VaciarCola(&colaEventosPC);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MostrarRegistroMovimientos(tEstado* estado)
{
    tLista nodoActual;
    tMovimientoLog* datosRegistro;
    int numeroPaso = 1, i;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    nodoActual = estado->Historial;

    system("cls");

    SetConsoleTextAttribute(hConsole, CLR_CYAN);
    printf("\n\t%c", 201);
    for(i = 0; i < 74; i++) printf("%c", 205);
    printf("%c\n", 187);

    printf("\t%c %-72s %c\n", 186, "                  BITACORA FINAL DE LA EXPLORACION", 186);

    printf("\t%c", 204);

    for(i = 0; i < 74; i++)
        printf("%c", 205);

    printf("%c\n", 185);

    if (nodoActual == NULL)
    {
        SetConsoleTextAttribute(hConsole, CLR_AMARILLO);
        printf("\t%c %-72s %c\n", 186, "                [!] LA BITACORA ESTA VACIA", 186);
    }
    else
    {

        while(nodoActual)
        {
            datosRegistro = (tMovimientoLog*)nodoActual->Info;

            SetConsoleTextAttribute(hConsole, CLR_CYAN);
            printf("\t%c ", 186);

            SetConsoleTextAttribute(hConsole, CLR_AMARILLO);
            printf("[%02d] ", numeroPaso++);

            SetConsoleTextAttribute(hConsole, 15);
            printf("Camara %-3d %c%c%c Camara %-3d",
                   datosRegistro->idOrigen, 196, 196, 16, datosRegistro->idDestino);

            SetConsoleTextAttribute(hConsole, 8);
            printf(" | ");

            SetConsoleTextAttribute(hConsole, 10);
            printf("%-35s    ", datosRegistro->descripcion);

            SetConsoleTextAttribute(hConsole, CLR_CYAN);
            printf(" %c\n", 186);

            nodoActual = nodoActual->sig;
        }
    }
    SetConsoleTextAttribute(hConsole, CLR_CYAN);
    printf("\t%c", 200);
    for(i = 0; i < 74; i++) printf("%c", 205);
    printf("%c\n", 188);

    SetConsoleTextAttribute(hConsole, CLR_AMARILLO);
    printf("\n\t>> Presione una tecla para volver");
    SetConsoleTextAttribute(hConsole, 7);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int TieneSolucion(tNodoArbolNario* nodo)
{
    tInfoCamara *info;
    tLista hijos;
    tNodoArbolNario *h;

    if(!nodo)
        return SIN_SOLUCION;
    info = (tInfoCamara*)nodo->info;

    /// Si la cámara tiene lava, este camino esta bloqueado
    if(info->hay_lava)
        return SIN_SOLUCION;

    /// si llegamos a la salida (S) y no hay lava, hay solucion
    if (info->es_salida)
        return HAY_SOLUCION;

    hijos = nodo->hijos;
    while (hijos)
    {
        h = *(tNodoArbolNario**)hijos->Info;
        if (TieneSolucion(h) == HAY_SOLUCION)
            return HAY_SOLUCION;
        hijos = hijos->sig;
    }

    return SIN_SOLUCION;
}
