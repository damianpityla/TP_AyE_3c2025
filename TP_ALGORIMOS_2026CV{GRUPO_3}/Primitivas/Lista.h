
#ifndef LISTA_H_INCLUDED
#define LISTA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TODO_OK     1
#define SIN_MEM    -1
#define VACIA      -2
#define DUPLICADO  -3

#define SIN_DUPLICADOS  0
#define CON_DUPLICADOS  1

#define MINIMO(X,Y)((X)<=(Y)?(X):(Y))

typedef int (*tCmp)(const void*, const void*);
typedef void (*tAccion)(const void*);
typedef void (*tAcum)(void *nodoInfo, const void *elem);
typedef int (*tCondicion)(const void* elem, const void* contexto);

typedef struct sNodoLista
{
    void* Info;
    unsigned TamInfo;
    struct sNodoLista* sig;
}tNodoLista;

typedef tNodoLista *tLista;

void    CrearLista                  (tLista* p);
void    VaciarLista                 (tLista* p);
int     sacarPrimeroLista           (tLista* pl, void* pd, unsigned tam);
void    RecorrerLista               (const tLista* pl, tAccion Accion);
int     InsertarEnOrdenLista        (tLista* pl, const void *Dato, unsigned Bytes, int Dup, tCmp Cmp, tAcum Acum);
int     BuscarPrimeroCondicionLista (tLista* p, void* Dato, unsigned Bytes, tCondicion Condicion, const void *Contexto);
int     PonerAlFinalEnLista         (tLista *p, const void *Dato, unsigned Bytes);
int     verPrimeroLista             (tLista *p, void *Dato, unsigned Bytes);

#endif // LISTA_H_INCLUDED

