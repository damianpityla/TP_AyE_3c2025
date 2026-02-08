#ifndef USUARIO_H_INCLUDED
#define USUARIO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Arbol.h"
#include "Lista.h"

#define ARCH_JUGADORES "Jugadores.dat"
#define ARCH_INDICE    "Jugadores.idx"
#define ARCH_PARTIDAS  "Partidas.dat"

#define TAM_NOMBRE 50

#define TODO_OK 1
#define ERROR_ARCHIVO -1

#define EPSILON 1e-4f



typedef struct
{
    int id;
    char nombre[TAM_NOMBRE + 1];
}tJugador;


typedef struct
{
    int idPartida;
    int idjugador;
    float Puntuacion;
    int CantMovimientos;
}tPartida;

typedef struct
{
    int id;
    char nombre[TAM_NOMBRE + 1];
    int desplazamiento;
}tIndiceJugador;

typedef struct
{
    int idJugador;
    char Nombre[TAM_NOMBRE + 1];
    float TotalPuntos;
    int partidas; ///ver si queremos guardar las partidas jugadas o no
}tRanking;

void AcumularRanking(void *nodoInfo, const void *elem);
void MostrarRanking(const void *a);
int CargarIndiceJugadores(const char *ArchJugadores, tArbolBin *pIndice);
int GenerarRanking(const char *ArchPartidas, tArbolBin *pIndice, tLista *pRanking);
int cmpRankingPorId(const void *a, const void *b);
int cmpIndicePorId(const void* a, const void* b);
int cmpRankingPorPuntosDesc(const void *a, const void *b);
#endif // USUARIO_H_INCLUDED
