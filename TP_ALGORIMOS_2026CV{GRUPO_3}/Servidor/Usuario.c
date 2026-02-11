#include "Usuario.h"
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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int obtenerUltimoIDJugador(const char* archJugadores)
{
    FILE* pf = fopen(archJugadores, "rb");
    tJugador jugador;
    if(!pf)
        return 0; // Si no existe el archivo, empezamos en 0

    fseek(pf, 0, SEEK_END);
    if(ftell(pf) == 0)
    {
        fclose(pf);
        return 0;
    }

    fseek(pf, (long)(-1 * sizeof(tJugador)), SEEK_END);
    fread(&jugador, sizeof(tJugador), 1, pf);
    fclose(pf);
    return jugador.id;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Guarda la partida en Partidas.dat (Requerimiento i)
int registrarPartidaEnServidor(const char* archPartidas, int idJugador, float puntos, int movimientos)
{
    tPartida nueva;
    FILE *pf = fopen(archPartidas, "ab"); // append binario
    if(!pf)
        return ERROR_ARCHIVO;

    // Podrías implementar obtenerUltimoIDPartida similar a la de jugadores
    nueva.idjugador = idJugador;
    nueva.Puntuacion = puntos;
    nueva.CantMovimientos = movimientos;

    fwrite(&nueva, sizeof(tPartida), 1, pf);
    fclose(pf);
    return TODO_OK;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int altaJugadorServidor(const char* archJugador, tArbolBin* pIndice, const char* nombre, tCmp cmp)
{
    int desplazamiento;
    FILE* pf = fopen(archJugador, "ab");
    if (!pf) return ERROR_ARCHIVO;

    tJugador nuevo;
    nuevo.id = obtenerUltimoIDJugador(archJugador) + 1;
    strncpy(nuevo.nombre, nombre, TAM_NOMBRE);
    nuevo.nombre[TAM_NOMBRE] = '\0';

    // 1. Guardar en el archivo de datos
    fseek(pf, 0, SEEK_END);
    desplazamiento = ftell(pf) / sizeof(tJugador);
    fwrite(&nuevo, sizeof(tJugador), 1, pf);
    fclose(pf);

    // 2. Insertar en el Árbol de Búsqueda Binaria (Índice)
    tIndiceJugador ind;
    ind.id = nuevo.id;
    ind.desplazamiento = desplazamiento;
    strcpy(ind.nombre, nuevo.nombre);

    return InsertarEnArbolBin(pIndice, &ind, sizeof(tIndiceJugador), cmp);
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int cmpIndicePorNombre(const void *a, const void *b)
{
    const tIndiceJugador *indA = (const tIndiceJugador*)a;
    const tIndiceJugador *indB = (const tIndiceJugador*)b;

    return stricmp(indA->nombre, indB->nombre);
}
