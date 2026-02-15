#ifndef USUARIO_H_INCLUDED
#define USUARIO_H_INCLUDED

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <conio.h>
#include <winsock2.h>
#include <windows.h>
#include <math.h>


#define OPC_OFFLINE 2
#define PIXEL ((unsigned char)254)

#include "../Config/Configuracion.h"
#include "../Primitivas/Arbol_Binario.h"
#include "../Primitivas/Arbol_N_ario.h"
#include "../Primitivas/Lista.h"
#include "../Primitivas/Cola.h"

#define ARCH_JUGADORES "Jugadores.dat"
#define ARCH_INDICE    "Jugadores.idx"
#define ARCH_PARTIDAS  "Partidas.dat"

#define TAM_NOMBRE 50

#define TODO_OK 1
#define ERROR_ARCHIVO -1

#define EPSILON 1e-4f


#define OPCONLINE 4
#define OPCOFFLINE 2

#define BUFFER_SIZE 1024
#define TAMCAMPO 50
#define TAMLINEA 256

#define TODOOK 1

#define LINEA ""

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


///////MENU OFFLINE
int MenuOffline(void);

void AcumularRanking(void *nodoInfo, const void *elem);
void MostrarRanking(const void *a);
int CargarIndiceJugadores(const char *ArchJugadores, tArbolBin *pIndice);
int GenerarRanking(const char *ArchPartidas, tArbolBin *pIndice, tLista *pRanking);
int cmpRankingPorId(const void *a, const void *b);
int cmpIndicePorId(const void* a, const void* b);
int cmpRankingPorPuntosDesc(const void *a, const void *b);


#endif // USUARIO_H_INCLUDED
