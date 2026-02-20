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

/// SEGURO ACA VAYA EL SOCKET PERO NO SE BIEN BIEN
int MenuOffLine()
{
    const char* opciones[OPC_OFFLINE] = { "JUGAR", "SALIR" };
    char linea[TAMLINEA];
    int opcion = 0, salir = 0;
    int i, j, len, leftPad, tecla, ext;
    const int ANCHO = 100;
    const char* MARGEN = "          ";
    char titulo[TAM_TITULO][101] = {
        "  ###### #   # #### ##### #####    #####  ##### #      #      # ##### #      #####  ###### ##    #  ",
        "      #  #   # #    #     #   #    #    # #     #      #      # #   # #     #     # #    # # #   #  ",
        "      #  #   # ###  # ### #   #    #    # ###   #       #    #  #   # #     #       ###### #  #  #  ",
        "  #   #  #   # #    #   # #   #    #    # #     #        #  #   #   # #     #     # #    # #   # #  ",
        "  #####  ##### #### ### # #####    #####  ##### #####     ##    ##### #####  #####  #    # #    ##  ",
        "                                                                                                   "
    };

    system("color 04");

    while(!salir)
    {
        system("cls");
        printf("\n");

        printf("%s%c", MARGEN, 201);
        for(j = 0; j < ANCHO; j++) printf("%c", 205);
        printf("%c\n", 187);

        printf("%s%c", MARGEN, 186);
        for(j = 0; j < ANCHO; j++) printf(" ");
        printf("%c\n", 186);

        for(i = 0; i < TAM_TITULO; i++)
        {
            len = strlen(titulo[i]);
            leftPad = (ANCHO - len) / 2;
            printf("%s%c", MARGEN, 186);
            for(j = 0; j < leftPad; j++) printf(" ");
            for(j = 0; j < len; j++) printf("%c",titulo[i][j]);
            for(j = leftPad + len; j < ANCHO; j++) printf(" ");
            printf("%c\n", 186);
        }

        printf("%s%c", MARGEN, 186);
        for(j = 0; j < ANCHO; j++) printf(" ");
        printf("%c\n", 186);

        leftPad = (ANCHO - TAM_ESTADO_OFFLINE) / 2;
        printf("%s%c", MARGEN, 186);
        for(j = 0; j < leftPad ; j++) printf(" ");
        printf("ESTADO OFFLINE");
        for(j = leftPad + TAM_ESTADO_OFFLINE; j < ANCHO; j++) printf(" ");
        printf("%c\n", 186);

        printf("%s%c", MARGEN, 186);
        for(j = 0; j < ANCHO; j++) printf(" ");
        printf("%c\n", 186);

        for(i = 0; i < OPC_OFFLINE; i++)
        {
            if(i == opcion)
                snprintf(linea, sizeof(linea), "-> %s", opciones[i]);
            else
                snprintf(linea, sizeof(linea), "   %s", opciones[i]);

            len = strlen(linea);
            leftPad = (ANCHO - len) / 2;
            printf("%s%c", MARGEN, 186);
            for(j = 0; j < leftPad; j++) printf(" ");
            printf("%s", linea);
            for(j = leftPad + len; j < ANCHO; j++) printf(" ");
            printf("%c\n", 186);
        }

        printf("%s%c", MARGEN, 186);
        for(j = 0; j < ANCHO; j++) printf(" ");
        printf("%c\n", 186);

        printf("%s%c", MARGEN, 200);
        for(j = 0; j < ANCHO; j++) printf("%c", 205);
        printf("%c\n", 188);

        tecla = getch();
        if (tecla == 0 || tecla == 224)
        {
            ext = getch();
            if (ext == 72) tecla = 'w';
            if (ext == 80) tecla = 's';
        }

        if (tolower(tecla) == 'w')
        {
            opcion--;
            if(opcion < 0) opcion = OPC_OFFLINE - 1;
        }
        else if (tolower(tecla) == 's')
        {
            opcion++;
            if(opcion >= OPC_OFFLINE) opcion = 0;
        }
        else if (tecla == 13)
        {
            if (opcion == 0)
            {
                tEstado estado;
                tConfig config;

                if (CargarConfiguracion(ARCH_CONFIG, &config) == TODO_OK)
                {
                    estado.Vidas = config.vidas_iniciales;
                    estado.Puntaje = 0;
                    estado.TurnosJugados = 0;
                    CrearLista(&(estado.MapaPadres));

                    if (GenerarEstructuraVolcan(&estado, &config))
                    {
                        ActualizarMapaPadres(estado.Volcan, &(estado.MapaPadres));
                        PoblarCamaras(&estado, &config);
                        EjecutarCicloJuego(&estado, &config, ARCH_CONFIG);

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
    return TODO_OK;
}
////********************************************************************************************************************************************************///
int MenuOnLine()
{
    const char* opciones[OPC_ONLINE] = { "JUGAR", "SALIR", "REGISTRARSE", "VER RANKING" };
    char linea[TAMLINEA];
    char nombre[50];
    char buffer[1024];
    char titulo[TAM_TITULO][101] = {
        "  ###### #   # #### ##### #####    #####  ##### #      #      # ##### #      #####  ###### ##    #  ",
        "      #  #   # #    #     #   #    #    # #     #      #      # #   # #     #     # #    # # #   #  ",
        "      #  #   # ###  # ### #   #    #    # ###   #       #    #  #   # #     #       ###### #  #  #  ",
        "  #   #  #   # #    #   # #   #    #    # #     #        #  #   #   # #     #     # #    # #   # #  ",
        "  #####  ##### #### ### # #####    #####  ##### #####     ##    ##### #####  #####  #    # #    ##  ",
        "                                                                                                   "
    };
    int opcion = 0;
    int salir = 0;
    int i = 0;
    int j = 0;
    int len = 0;
    int leftPad = 0;
    int tecla = 0;
    int ext = 0;
    int bytes = 0;
    int idJugador = 0;
    const int ANCHO = 100;
    const char* MARGEN = "          ";
    SOCKET sock;
    SOCKET sockReg;
    SOCKET sockR;
    tEstado estado;
    tConfig config;
    tRanking reg;

    system("color 02");

    while (!salir)
    {
        system("cls");
        printf("\n");
        printf("%s%c", MARGEN, 201);
        for (j = 0; j < ANCHO; j++) printf("%c", 205);
        printf("%c\n", 187);
        printf("%s%c", MARGEN, 186);
        for (j = 0; j < ANCHO; j++) printf(" ");
        printf("%c\n", 186);

        for (i = 0; i < TAM_TITULO; i++)
        {
            len = strlen(titulo[i]);
            leftPad = (ANCHO - len) / 2;
            printf("%s%c", MARGEN, 186);
            for (j = 0; j < leftPad; j++) printf(" ");
            for (j = 0; j < len; j++) printf("%c", titulo[i][j]);
            for (j = leftPad + len; j < ANCHO; j++) printf(" ");
            printf("%c\n", 186);
        }

        printf("%s%c", MARGEN, 186);
        for (j = 0; j < ANCHO; j++) printf(" ");
        printf("%c\n", 186);

        leftPad = (ANCHO - TAM_ESTADO_ONLINE) / 2;
        printf("%s%c", MARGEN, 186);
        for (j = 0; j < leftPad; j++) printf(" ");
        printf("ESTADO ONLINE");
        for (j = leftPad + TAM_ESTADO_ONLINE; j < ANCHO; j++) printf(" ");
        printf("%c\n", 186);

        printf("%s%c", MARGEN, 186);
        for (j = 0; j < ANCHO; j++) printf(" ");
        printf("%c\n", 186);

        for (i = 0; i < OPC_ONLINE; i++)
        {
            if (i == opcion)
                snprintf(linea, sizeof(linea), "-> %s", opciones[i]);
            else
                snprintf(linea, sizeof(linea), "   %s", opciones[i]);

            len = strlen(linea);
            leftPad = (ANCHO - len) / 2;
            printf("%s%c", MARGEN, 186);
            for (j = 0; j < leftPad; j++) printf(" ");
            printf("%s", linea);
            for (j = leftPad + len; j < ANCHO; j++) printf(" ");
            printf("%c\n", 186);
        }

        printf("%s%c", MARGEN, 186);
        for (j = 0; j < ANCHO; j++) printf(" ");
        printf("%c\n", 186);

        printf("%s%c", MARGEN, 200);
        for (j = 0; j < ANCHO; j++) printf("%c", 205);
        printf("%c\n", 188);

        tecla = getch();
        if (tecla == 0 || tecla == 224)
        {
            ext = getch();
            if (ext == 72) tecla = 'w';
            if (ext == 80) tecla = 's';
        }

        if (tolower(tecla) == 'w')
        {
            opcion--;
            if (opcion < 0) opcion = OPC_ONLINE - 1;
        }
        else if (tolower(tecla) == 's')
        {
            opcion++;
            if (opcion >= OPC_ONLINE) opcion = 0;
        }
        else if (tecla == 13)
        {
            switch (opcion)
            {
            case 0:
                system("cls");
                printf("\n--- CONECTANDO AL SERVIDOR ---\n");
                iniciar_entorno_socket();
                sock = conectar_servidor("127.0.0.1", 8080);

                if (sock != INVALID_SOCKET)
                {
                    printf("Ingrese su nombre de usuario para el ranking: ");
                    scanf("%s", nombre);

                    sprintf(buffer, "LOGIN %s", nombre);
                    send(sock, buffer, (int)strlen(buffer), 0);

                    bytes = recv(sock, buffer, sizeof(buffer) - 1, 0);
                    if (bytes > 0)
                    {
                        buffer[bytes] = '\0';
                        if (strncmp(buffer, "OK", 2) == 0)
                        {
                            idJugador = atoi(buffer + 3);
                            printf("Login exitoso. ID asignado: %d\n", idJugador);
                            Sleep(1000);

                            if (CargarConfiguracion(ARCH_CONFIG, &config) == TODO_OK)
                            {
                                estado.Vidas = config.vidas_iniciales;
                                estado.Puntaje = 0;
                                estado.TurnosJugados = 0;
                                CrearLista(&(estado.MapaPadres));

                                if (GenerarEstructuraVolcan(&estado, &config))
                                {
                                    ActualizarMapaPadres(estado.Volcan, &(estado.MapaPadres));
                                    PoblarCamaras(&estado, &config);
                                    EjecutarCicloJuego(&estado, &config, ARCH_CONFIG);
                                    enviar_partida(sock, idJugador, (float)estado.Puntaje, estado.TurnosJugados);

                                    VaciarArbolNario(&(estado.Volcan));
                                    VaciarLista(&(estado.MapaPadres));
                                }
                            }
                        }
                        else
                        {
                            printf("\nError: Usuario no registrado o no encontrado.\n");
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
                system("color 02");
                break;

            case 1:
                system("cls");
                printf("\n\n\n\t\t\tSaliendo...\n");
                Sleep(500);
                salir = 1;
                break;

            case 2:
                system("cls");
                printf("\n--- REGISTRO DE NUEVO EXPLORADOR ---\n");
                iniciar_entorno_socket();
                sockReg = conectar_servidor("127.0.0.1", 8080);
                if (sockReg != INVALID_SOCKET)
                {
                    printf("Ingrese el nombre de usuario a registrar: ");
                    scanf("%s", nombre);
                    sprintf(buffer, "REGISTER %s", nombre);
                    send(sockReg, buffer, (int)strlen(buffer), 0);
                    bytes = recv(sockReg, buffer, sizeof(buffer) - 1, 0);
                    if (bytes > 0)
                    {
                        buffer[bytes] = '\0';
                        if (strcmp(buffer, "REGISTER_OK") == 0)
                            printf("\n¡Usuario '%s' registrado con exito!\n", nombre);
                        else if (strcmp(buffer, "ERROR_ALREADY_EXISTS") == 0)
                            printf("\nError: El usuario ya existe.\n");
                        else
                            printf("\nError en el servidor.\n");
                    }
                    closesocket(sockReg);
                }
                else printf("\nError: Servidor no disponible.\n");
                printf("\nPresione una tecla para volver...");
                getch();
                system("color 02");
                break;

            case 3:
                system("cls");
                sockR = conectar_servidor("127.0.0.1", 8080);
                if (sockR != INVALID_SOCKET)
                {
                    send(sockR, "RANKING", 7, 0);
                    printf("--- RANKING GLOBAL (DESDE SERVIDOR) ---\n");
                    printf("%-5s %-20s %-10s %-10s\n", "ID", "NOMBRE", "PUNTOS", "PARTIDAS");
                    while (recv(sockR, (char*)&reg, sizeof(tRanking), 0) > 0 && reg.idJugador != -1)
                    {
                        printf("%-5d %-20s %-10.2f %-10d\n", reg.idJugador, reg.Nombre, reg.TotalPuntos, reg.partidas);
                    }
                    closesocket(sockR);
                }
                else printf("Servidor no disponible.\n");
                getch();
                break;
            }
        }
    }
    return TODO_OK;
}
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

