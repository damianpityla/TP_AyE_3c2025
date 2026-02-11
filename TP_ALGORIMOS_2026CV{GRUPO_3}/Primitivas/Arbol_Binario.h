#ifndef ARBOLBINARIO_H_INCLUDED
#define ARBOLBINARIO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TODO_OK 1
#define SIN_MEM -2
#define DUPLICADO -3
#define ARBOL_VACIO -4

#define MIN(X,Y)((X) < (Y) ?(X) : (Y))

typedef struct sNodoArbolBin
{
    void *Info;
    unsigned TamInfo;
    struct sNodoArbolBin *Izq,
                         *Der;
}tNodoArbolBin;
typedef tNodoArbolBin *tArbolBin;

typedef void (*tAccion)(const void* elem);
typedef int  (*tCmp)(const void* a, const void* b);

void CrearArbolBin(tArbolBin *p);
void VaciarArbolBin(tArbolBin* p);
int InsertarEnArbolBin(tArbolBin* p, const void *Dato, unsigned Bytes, tCmp cmp);
tArbolBin* BusquedaEnArbolBin(tArbolBin* p, const void *Dato, tCmp cmp);

#endif // ARBOLBINARIO_H_INCLUDED
