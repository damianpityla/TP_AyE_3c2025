#include "Usuario.h"
#include "../Servidor/Servidor.h"
////********************************************************************************************************************************************************///

//-------------------MENU ONLINE-------------//

//-------------------MENU OFFLINE------------------------//

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
    for(col = 0; col < 60; col++) printf("%c", 205);
    printf("%c\n", 188);
    SetConsoleTextAttribute(hConsole, 7);
}

int MenuOffLine()
{
    int opcion = 0, salir = 0, tecla;
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
                if (CargarConfiguracion("config.txt", &config) == 0)
                {
                    estado.Vidas = config.vidas_iniciales;
                    estado.Puntaje = 0;
                    estado.TurnosJugados = 0;
                    CrearLista(&(estado.MapaPadres));

                    if (GenerarEstructuraVolcan(&estado, &config))
                    {
                        ActualizarMapaPadres(estado.Volcan, &(estado.MapaPadres));
                        PoblarCamaras(&estado, &config);

                        EjecutarCicloJuego(&estado, &config, "config.txt");

                        VaciarArbolNario(&(estado.Volcan));
                        VaciarLista(&(estado.MapaPadres));
                    }
                }
                system("color 04");
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
    int opcion = 0, salir = 0, tecla, bytes, idJugador;
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
            case 0:
                system("cls");
                printf("\n--- INICIAR PARTIDA ONLINE ---\n");
                printf("Ingrese nombre de usuario: ");
                scanf("%s", nombre);
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
                            if (CargarConfiguracion("config.txt", &config) == TODO_OK)
                            {
                                estado.Vidas = config.vidas_iniciales;
                                estado.Puntaje = 0;
                                estado.TurnosJugados = 0;
                                CrearLista(&(estado.MapaPadres));
                                if (GenerarEstructuraVolcan(&estado, &config))
                                {
                                    ActualizarMapaPadres(estado.Volcan, &(estado.MapaPadres));
                                    PoblarCamaras(&estado, &config);
                                    EjecutarCicloJuego(&estado, &config, "config.txt");
                                    enviar_partida(sock, idJugador, (float)estado.Puntaje, estado.TurnosJugados);

                                    VaciarArbolNario(&(estado.Volcan));
                                    VaciarLista(&(estado.MapaPadres));
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
                break;

            case 1:
                system("cls");
                printf("\n--- REGISTRO DE NUEVO EXPLORADOR ---\n");
                sock = conectar_servidor("127.0.0.1", 8080);
                if (sock != INVALID_SOCKET)
                {
                    printf("Ingrese el nombre de usuario a registrar: ");
                    scanf("%s", nombre);
                    sprintf(buffer, "REGISTER %s", nombre);
                    send(sock, buffer, (int)strlen(buffer), 0);
                    bytes = recv(sock, buffer, sizeof(buffer) - 1, 0);
                    if (bytes > 0)
                    {
                        buffer[bytes] = '\0';
                        if (strcmp(buffer, "REGISTER_OK") == 0)
                        {
                            printf("\nUsuario '%s' registrado con exito!\n", nombre);
                        }
                        else if (strcmp(buffer, "ERROR_ALREADY_EXISTS") == 0)
                        {
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
                printf("\nPresione una tecla para volver...");
                getch();
                break;

            case 2:
                system("cls");
                sock = conectar_servidor("127.0.0.1", 8080);
                if (sock != INVALID_SOCKET)
                {
                    send(sock, "RANKING", 7, 0);
                    printf("\n--- RANKING GLOBAL (DESDE SERVIDOR) ---\n");
                    printf("%-5s %-20s %-10s %-10s\n", "ID", "NOMBRE", "PUNTOS", "PARTIDAS");
                    while (recv(sock, (char*)&reg, sizeof(tRanking), 0) > 0 && reg.idJugador != -1)
                    {
                        printf("%-5d %-20s %-10.2f %-10d\n", reg.idJugador, reg.Nombre, reg.TotalPuntos, reg.partidas);
                    }
                    closesocket(sock);
                }
                else
                {
                    printf("\nError: Servidor no disponible.\n");
                }
                printf("\nPresione una tecla para volver...");
                getch();
                break;

            case 3:
                salir = 1;
                break;
            }
        }
    }
    return TODO_OK;
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
