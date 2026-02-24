#include "Usuario.h"
#include "../Servidor/Servidor.h"
#define OPC_OFFLINE 2

int Menu()
{
    if (iniciar_entorno_socket() != 0)
    {
        return MenuOffLine();
    }

    SOCKET sock = conectar_servidor("127.0.0.1", 8080);

    if (sock == INVALID_SOCKET)
    {
        WSACleanup();
        return MenuOffLine();
    }

    closesocket(sock);
    return MenuOnLine();
}
void MostrarMenuGeneral(int opcionSel, int tipoMenu)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int col, i;
    int colorPrincipal = (tipoMenu == 1) ? 12 : 10;
    const char* estadoStr = (tipoMenu == 1) ? "ESTADO: OFFLINE" : "ESTADO: ONLINE";
    const char* opcOff[] = { "ENTRAR AL VOLCAN", "ABANDONAR MISION" };
    const char* opcOn[]  = { "INICIAR PARTIDA", "REGISTRARSE", "VER RANKING", "DESCONECTAR" };
    int cantOpc = (tipoMenu == 1) ? 2 : 4;

    system("cls");
    SetConsoleTextAttribute(hConsole, colorPrincipal | FOREGROUND_INTENSITY);

    printf("\t%c", 201);
    for(col = 0; col < 60; col++) printf("%c", 205);
    printf("%c\n", 187);

    SetConsoleTextAttribute(hConsole, colorPrincipal | FOREGROUND_INTENSITY);
    printf("\t%c  __      __  ____   _        _____           _   _         %c\n", 186, 186);
    printf("\t%c  \\ \\    / / / __ \\ | |      / ____|    /\\   | \\ | |        %c\n", 186, 186);
    printf("\t%c   \\ \\  / / | |  | || |     | |        /  \\  |  \\| |        %c\n", 186, 186);
    printf("\t%c    \\ \\/ /  | |  | || |     | |       / /\\ \\ | . ` |        %c\n", 186, 186);
    printf("\t%c     \\  /   | |__| || |____ | |____  / ____ \\| |\\  |        %c\n", 186, 186);
    printf("\t%c      \\/     \\____/ |______| \\_____|/_/    \\_\\_| \\_|        %c\n", 186, 186);

    SetConsoleTextAttribute(hConsole, colorPrincipal);
    printf("\t%c", 204);
    for(col = 0; col < 60; col++) printf("%c", 205);
    printf("%c\n", 185);

    printf("\t%c                %-44s%c\n", 186, estadoStr, 186);

    for(i = 0; i < cantOpc; i++) {
        printf("\t%c", 186);
        if(i == opcionSel) {
            SetConsoleTextAttribute(hConsole, 14 | FOREGROUND_INTENSITY);
            printf("  -> %-54s ", (tipoMenu == 1) ? opcOff[i] : opcOn[i]);
        } else {
            SetConsoleTextAttribute(hConsole, 7);
            printf("     %-54s ", (tipoMenu == 1) ? opcOff[i] : opcOn[i]);
        }
        SetConsoleTextAttribute(hConsole, colorPrincipal);
        printf("%c\n", 186);
    }

    printf("\t%c", 200);
    for(col = 0; col < 60; col++)
        printf("%c", 205);
    printf("%c\n", 188);
    SetConsoleTextAttribute(hConsole, 7);
}

int MenuOffLine()
{
    int opcion = 0, salir = 0, tecla, mapaPobre;
    tEstado estado;
    tConfig config;

    while(!salir)
    {
        MostrarMenuGeneral(opcion, 1);
        tecla = getch();

        if (tecla == 0 || tecla == 224)
        {
            tecla = getch();
            if (tecla == 72) opcion = 0;
            if (tecla == 80) opcion = 1;
            Beep(600, 40);
        }
        else if (tecla == 13)
        {
            Beep(800, 100);

            if (opcion == 0)
            {
                system("cls");
                printf("\n Generando volcan...");

                config.altura_maxima = 4;
                config.max_conexiones = 3;
                config.vidas_iniciales = 3;
                config.criaturas = 2;
                config.premios = 1;

                CargarConfiguracion(ARCH_CONFIG, &config);

                mapaPobre = 1;
                while (mapaPobre)
                {
                    estado.Vidas = config.vidas_iniciales;
                    estado.Puntaje = 0;
                    estado.TurnosJugados = 0;
                    CrearLista(&(estado.MapaPadres));

                    if (GenerarEstructuraVolcan(&estado, &config))
                    {
                        ActualizarMapaPadres(estado.Volcan, &(estado.MapaPadres));

                        if (estado.Volcan != NULL && estado.Volcan->hijos != NULL)
                        {
                            mapaPobre = 0;
                            PoblarCamaras(&estado, &config);

                            PosicionarJugadorEnInicio(&estado);

                            GrabarArchivoVolcan(estado.Volcan, &estado, ARCH_VOLCAN);
                            EjecutarCicloJuego(&estado, &config, ARCH_CONFIG);
                        }

                        VaciarArbolNario(&(estado.Volcan));
                        VaciarLista(&(estado.MapaPadres));
                    }
                    else
                    {
                        mapaPobre = 0;
                    }
                }
                system("color 07");
            }
            else
            {
                salir = 1;
            }
        }
    }
    return 0;
}
////********************************************************************************************************************************************************///
int MenuOnLine()
{
    int opcion = 0, salir = 0, tecla, bytes, idJugador, i, puesto, mapaPobreOnline;
    char nombre[50], buffer[1024];
    SOCKET sock;
    tEstado estado;
    tConfig config;
    tRanking reg;

    while (!salir)
    {
        MostrarMenuGeneral(opcion, 2);
        tecla = getch();

        if (tecla == 0 || tecla == 224)
        {
            tecla = getch();
            if (tecla == 72) opcion = (opcion > 0) ? opcion - 1 : 3;
            if (tecla == 80) opcion = (opcion < 3) ? opcion + 1 : 0;
            Beep(600, 40);
        }
        else if (tecla == 13)
        {
            Beep(800, 100);
            switch (opcion)
            {
                case OPC_INICIAR_PARTIDA:
                system("cls");
                if(SolicitarDatoUsuario(nombre, "INICIAR PARTIDA ONLINE"))
                {
                    sock = conectar_servidor("127.0.0.1", 8080);
                    if (sock != INVALID_SOCKET)
                    {
                        sprintf(buffer, "LOGIN %s", nombre);
                        send(sock, buffer, (int)strlen(buffer), 0);
                        bytes = recv(sock, buffer, sizeof(buffer) - 1, 0);

                        if (bytes > 0)
                        {
                            buffer[bytes] = '\0';
                            if (strncmp(buffer, "OK", 2) == 0)
                            {
                                idJugador = atoi(buffer + 3);

                                config.altura_maxima = 4;
                                config.max_conexiones = 3;
                                config.vidas_iniciales = 3;
                                config.criaturas = 2;
                                config.premios = 1;

                                CargarConfiguracion(ARCH_CONFIG, &config);

                                mapaPobreOnline = 1;
                                while(mapaPobreOnline)
                                {
                                    estado.Vidas = config.vidas_iniciales;
                                    estado.Puntaje = 0;
                                    estado.TurnosJugados = 0;
                                    CrearLista(&(estado.MapaPadres));

                                    if (GenerarEstructuraVolcan(&estado, &config))
                                    {
                                        ActualizarMapaPadres(estado.Volcan, &(estado.MapaPadres));

                                        if (estado.Volcan != NULL && estado.Volcan->hijos != NULL)
                                        {
                                            mapaPobreOnline = 0; // Salimos del bucle
                                            PoblarCamaras(&estado, &config);

                                            PosicionarJugadorEnInicio(&estado);

                                            GrabarArchivoVolcan(estado.Volcan, &estado, ARCH_VOLCAN);
                                            EjecutarCicloJuego(&estado, &config, ARCH_CONFIG);

                                            enviar_partida(sock, idJugador, (float)estado.Puntaje, estado.TurnosJugados);
                                        }

                                        VaciarArbolNario(&(estado.Volcan));
                                        VaciarLista(&(estado.MapaPadres));
                                    }
                                    else
                                    {
                                        mapaPobreOnline = 0;
                                    }
                                }
                            }
                            else
                            {
                                printf("\nError: Usuario no registrado en el servidor.\n");
                            }
                        }
                        closesocket(sock);
                    }
                    else
                    {
                        printf("\nError: El servidor no responde.\n");
                    }
                    printf("\nPresione una tecla para volver...");
                    getch();
                }
                break;

            case OPC_REGISTRARSE:
                if(SolicitarDatoUsuario(nombre, "REGISTRO DE EXPLORADOR"))
                {
                sock = conectar_servidor("127.0.0.1", 8080);
                if (sock != INVALID_SOCKET)
                {
                    sprintf(buffer, "REGISTER %s", nombre);
                    send(sock, buffer, (int)strlen(buffer), 0);
                    bytes = recv(sock, buffer, sizeof(buffer) - 1, 0);
                    if (bytes > 0)
                    {
                        buffer[bytes] = '\0';
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14 | FOREGROUND_INTENSITY);
                        if (strcmp(buffer, "REGISTER_OK") == 0)
                        {
                            printf("\nUsuario '%s' registrado con exito!\n", nombre);
                        }
                        else if (strcmp(buffer, "ERROR_ALREADY_EXISTS") == 0)
                        {
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12 | FOREGROUND_INTENSITY);
                            printf("\nError: El usuario ya existe.\n");
                        }
                        else
                        {
                            printf("\nError en la respuesta del servidor.\n");
                        }
                    }
                    closesocket(sock);
                }
                else
                {
                    printf("\nError: Servidor no disponible.\n");
                }
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11 | FOREGROUND_INTENSITY);
                printf("\n Presione una tecla para volver...");
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                getch();}
                break;


                case OPC_VER_RANKING:
                system("cls");
                sock = conectar_servidor("127.0.0.1", 8080);
                if (sock != INVALID_SOCKET)
                {
                    HANDLE hConsola = GetStdHandle(STD_OUTPUT_HANDLE);
                    send(sock, "RANKING", 7, 0);

                    SetConsoleTextAttribute(hConsola, 11 | FOREGROUND_INTENSITY);
                    printf("\n\t%c", 201);
                    for(i = 0; i < 62; i++)
                        printf("%c", 205);
                    printf("%c\n", 187);

                    printf("\t%c %-60s %c\n", 186, "                HALL OF FAME - EXPLORADORES", 186);

                    if (recv(sock, (char*)&reg, sizeof(tRanking), 0) > 0 && reg.idJugador != -1)
                    {
                        printf("\t%c", 204);
                        for(i = 0; i < 62; i++) printf("%c", 205);
                        printf("%c\n", 185);

                        printf("\t%c  %-4s | %-22s | %-10s | %-14s %c\n", 186, "ID", "NOMBRE", "PUNTOS", "PARTIDAS", 186);

                        puesto = 1;
                        do {
                            printf("\t%c", 204);
                            for(int i = 0; i < 62; i++) printf("%c", 205);
                            printf("%c\n", 185);

                            SetConsoleTextAttribute(hConsola, 11 | FOREGROUND_INTENSITY);
                            printf("\t%c  ", 186);

                            if (puesto == 1) SetConsoleTextAttribute(hConsola, 14 | FOREGROUND_INTENSITY);
                            else if (puesto == 2) SetConsoleTextAttribute(hConsola, 7 | FOREGROUND_INTENSITY);
                            else if (puesto == 3) SetConsoleTextAttribute(hConsola, 6);
                            else SetConsoleTextAttribute(hConsola, 11 | FOREGROUND_INTENSITY);

                            printf("%-4d | %-22s | %-10.2f | %-14d ",
                                   reg.idJugador, reg.Nombre, reg.TotalPuntos, reg.partidas);

                            SetConsoleTextAttribute(hConsola, 11 | FOREGROUND_INTENSITY);
                            printf("%c\n", 186);
                            puesto++;
                        } while (recv(sock, (char*)&reg, sizeof(tRanking), 0) > 0 && reg.idJugador != -1);
                    }
                    else
                    {
                        printf("\t%c", 204);
                        for(i = 0; i < 62; i++)
                            printf("%c", 205);
                        printf("%c\n", 185);

                        SetConsoleTextAttribute(hConsola, 11 | FOREGROUND_INTENSITY);
                        printf("\t%c %-60s %c\n", 186, "       EL RANKING ESTA VACIO - SE EL PRIMERO!", 186);
                    }

                    SetConsoleTextAttribute(hConsola, 11 | FOREGROUND_INTENSITY);
                    printf("\t%c", 200);
                    for(i = 0; i < 62; i++) printf("%c", 205);
                    printf("%c\n", 188);

                    SetConsoleTextAttribute(hConsola, 7);
                    closesocket(sock);
                }
                printf("\nPresione una tecla para volver...");
                getch();
                break;

            case OPC_DESCONECTAR:
                salir = 1;
                break;
            }
        }
    }
    return TODO_OK;
}
int SolicitarDatoUsuario(char *bufferDato, const char *titulo)
{
    HANDLE hConsola = GetStdHandle(STD_OUTPUT_HANDLE);
    WORD colorLineas = 11 | FOREGROUND_INTENSITY;
    WORD colorTexto = 14 | FOREGROUND_INTENSITY;
    char *p = bufferDato;
    char tecla = 0;

    system("cls");

    SetConsoleTextAttribute(hConsola, colorLineas);
    printf("==================================================\n");
    printf("|| ");
    SetConsoleTextAttribute(hConsola, colorTexto);
    printf(" %-43s", titulo);
    SetConsoleTextAttribute(hConsola, colorLineas);
    printf(" ||\n");
    printf("==================================================\n");
    printf("||                                              ||\n");
    printf("|| ");
    SetConsoleTextAttribute(hConsola, colorTexto);
    printf(" Ingrese nombre (ESC para cancelar):        ");
    SetConsoleTextAttribute(hConsola, colorLineas);
    printf(" ||\n");
    printf("|| > ");

    SetConsoleTextAttribute(hConsola, colorTexto);

    while (tecla != 13 && tecla != 27)
    {
        tecla = getch();

        if (tecla == 27) return 0;

        if (tecla == 8)
        {
            if (p > bufferDato)
            {
                p--;
                printf("\b \b");
            }
        }
        else if (tecla != 13 && (p - bufferDato) < 49)
        {
            *p = tecla;
            printf("%c", *p);
            p++;
        }
    }

    *p = '\0';

    SetConsoleTextAttribute(hConsola, colorLineas);
    printf("\n||                                              ||\n");
    printf("==================================================\n");

    return (p > bufferDato);
}
////********************************************************************************************************************************************************///
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
