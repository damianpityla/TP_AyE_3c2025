#ifndef CRIATURA_H_INCLUDED
#define CRIATURA_H_INCLUDED

#include "../Primitivas/Arbol_N_ario.h"
#include "../Primitivas/Cola.h"
#include "Juego.h"
#include "Volcan.h"


struct sEstado;

typedef void (*tAccionNodo)(tNodoArbolNario* Nodo, void* Contexto);

void MoverCriaturas(tNodoArbolNario* raiz, tEstado* estado);
void RecorrerBFS(tNodoArbolNario *Raiz, tAccionNodo Accion, void *Contexto);
void RecorrerDFSpreorden(const tNodoArbolNario *Nodo, tAccionNodo Accion, void *Contexto);

int EsAncestro(tNodoArbolNario* potencialAncestro, tNodoArbolNario* objetivo);
int EsAncestroPorId(tNodoArbolNario* potencialAncestro, int idObjetivo);


#endif
