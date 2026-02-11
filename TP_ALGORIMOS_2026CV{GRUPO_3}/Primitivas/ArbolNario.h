#ifndef ARBOL_H_INCLUDED
#define ARBOL_H_INCLUDED


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Lista.h"

#define TODO_OK 1
#define SIN_MEM -2
#define DUPLICADO -3
#define ARBOL_VACIO -4

#define MINIMO(X,Y)((X)<=(Y)?(X):(Y))

typedef struct sNodoArbolNario
{
    void* info;
    unsigned tamInfo;
    tLista hijos;          // lista de (tNodoArbolNA*)  -> punteros a hijos
}tNodoArbolNario;
typedef tNodoArbolNario* tArbolNario;


void CrearArbolNario(tArbolNario* pa);
void VaciarArbolNario(tArbolNario* pa);

int CrearNodoArbolNario(tArbolNario* pa, const void* pd, unsigned tam);

/*
typedef struct sNodoArbolBin
{
    void* info;
    unsigned tamInfo;
    struct sNodoArbolBin* hijoIzq;
    struct sNodoArbolBin* hijoDer;
}tNodoArbolBin;

typedef tNodoArbolBin* tArbolBin;
*/
/*
typedef int (*tCmp)(const void*, const void*);
typedef void (*tAccion)(const void*);

int InsertarEnArbolRecursivo(tArbolBin *p, const void* Dato, unsigned Bytes, tCmp cmp);
void VaciarArbolBin(tArbolBin* p);
void CrearArbolBin(tArbolBin* p);
tArbolBin* BusquedaEnArbolRecursivo(tArbolBin *p, const void* Dato, tCmp cmp);
*/

#endif // ARBOL_H_INCLUDED
