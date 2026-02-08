
#ifndef LISTA_H_INCLUDED
#define LISTA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TODO_OK     1
#define SIN_MEM    -2
#define DUPLICADO  -3

#define SIN_DUPLICADOS  0
#define CON_DUPLICADOS  1

typedef int (*tCmp)(const void*, const void*);
typedef void (*tAccion)(const void*);
typedef void (*tAcum)(void *nodoInfo, const void *elem);

typedef struct sNodoLista
{
    void* Info;
    unsigned TamInfo;
    struct sNodoLista* sig;
} tNodoLista;

typedef tNodoLista *tLista;

void CrearLista(tLista* p);
void VaciarLista(tLista* p);
void RecorrerLista(const tLista* pl, tAccion Accion);
int InsertarEnOrdenLista(tLista* pl, const void *Dato, unsigned Bytes, int Dup, tCmp Cmp, tAcum Acum);
int OrdenarLista(tLista *p, tCmp Cmp);
tNodoLista* BuscarAnteriorDelMenor(tLista* lista, tCmp comparar);

#endif // LISTA_H_INCLUDED

