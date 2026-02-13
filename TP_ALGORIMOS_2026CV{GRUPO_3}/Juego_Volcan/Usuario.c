#include "Usuario.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void activarColores() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;

    DWORD dwMode = 0;
    // Agregamos (HANDLE) para asegurar compatibilidad
    GetConsoleMode(hOut, &dwMode);

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}

void Menu_Offline()
{
    int altura = 50;
    int ancho = 50;
    activarColores();
    printf("\x1b[96m");

    printf("%c", 218);
    for(int i = 0; i < ancho; i++) printf("%c", 196);
    printf("%c\n", 191);

    for(int i = 0; i < altura; i++)
    {
        printf("%c", 179);
        for(int j = 0; j < ancho; j++) printf(" ");
        printf("%c\n", 179);
    }
    printf("%c", 192);
    for(int i = 0; i < ancho; i++) printf("%c", 196);
    printf("%c\n", 217);
    printf("\x1b[0m");
}

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

