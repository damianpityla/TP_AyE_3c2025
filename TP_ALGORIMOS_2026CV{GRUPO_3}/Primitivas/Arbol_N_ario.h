#ifndef ARBOL_H_INCLUDED
#define ARBOL_H_INCLUDED


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Lista.h"
#include "Cola.h"

#define TODO_OK 1
#define SIN_MEM -2
#define DUPLICADO -3
#define ARBOL_VACIO -4

#define HOJA 98
#define NO_HOJA 99

#define MINIMO(X,Y)((X)<=(Y)?(X):(Y))

typedef struct sNodoArbolNario
{
    void* info;
    unsigned tamInfo;
    tLista hijos;  // lista de (tNodoArbolNA*)  -> punteros a hijos
}tNodoArbolNario;
typedef tNodoArbolNario* tArbolNario;


void CrearArbolNario(tArbolNario* p);
void VaciarArbolNario(tArbolNario* p);

int CrearNodoArbolNario(tArbolNario* p, const void *Dato, unsigned Bytes);
int EsHoja(const tNodoArbolNario* Nodo);
int HijoEnPos(const tNodoArbolNario *actual, int pos, tNodoArbolNario **destino);
tNodoArbolNario *ObtenerPrimeraHoja(tNodoArbolNario *p);


#endif // ARBOL_H_INCLUDED
