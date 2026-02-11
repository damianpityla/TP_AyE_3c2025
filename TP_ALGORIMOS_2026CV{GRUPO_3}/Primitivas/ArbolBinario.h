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
    void* info;
    unsigned tamInfo;
    struct sNodoArbolBin *Izq,
                         *Der;
}tNodoArbolBin;
typedef tNodoArbolBin *tArbolBin;


void CrearArbolBin(tArbolBin *p);
void VaciarArbolBin(tArbolBin* p);

#endif // ARBOLBINARIO_H_INCLUDED
