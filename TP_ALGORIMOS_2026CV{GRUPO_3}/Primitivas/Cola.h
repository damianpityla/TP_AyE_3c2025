#ifndef COLA_H_INCLUDED
#define COLA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MINIMO(X,Y)((X)<=(Y)?(X):(Y))

typedef struct sNodoCola{
    void* info;
    unsigned tamInfo;
    struct sNodoCola* sig;
}tNodoCola;
typedef struct{
    tNodoCola *pri,
              *ult;
}tCola;

void crearCola(tCola* pc);
int colaLlena(const tCola* pc, unsigned tam);
int ponerEnCola(tCola* pc, const void* pd, unsigned tam);
int verPrimeroCola(const tCola* pc, void* pd, unsigned tam);
int colaVacia(const tCola* pc);
int sacarDeCola(tCola* pc, void* pd, unsigned tam);
void vaciarCola(tCola* pc);


#endif // COLA_H_INCLUDED
