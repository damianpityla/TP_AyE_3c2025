#ifndef COLA_H_INCLUDED
#define COLA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TODO_OK 1
#define SIN_MEM -1
#define VACIA -2
#define MIN(X,Y)((X) < (Y) ? (X) : (Y))

typedef struct sNodoCola{
    void* info;
    unsigned tamInfo;
    struct sNodoCola *sig;
}tNodoCola;

typedef struct
{
    tNodoCola *pri,
              *ult;
}tCola;

void    CrearCola       (tCola* pc);
int     PonerEnCola     (tCola* pc, const void* pd, unsigned tam);
int     ColaVacia       (tCola* pc);
int     SacarDeCola     (tCola* pc, void* pd, unsigned tam);
void    VaciarCola      (tCola* pc);


#endif // COLA_H_INCLUDED
