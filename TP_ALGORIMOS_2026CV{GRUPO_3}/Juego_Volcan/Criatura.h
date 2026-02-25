#ifndef CRIATURA_H_INCLUDED
#define CRIATURA_H_INCLUDED

#include <windows.h>
#include "Volcan.h"
#include "../Primitivas/Cola.h"

typedef void (*tAccionNodo)(tNodoArbolNario* Nodo, void* Contexto);

void                MoverCriaturas                     (tNodoArbolNario* raiz, tEstado* estado);
int                 EsAncestro                         (tNodoArbolNario *potencialAncestro, tNodoArbolNario* objetivo);
void                VerificarCombate                   (tEstado *estado);
tNodoArbolNario    *DeterminarDireccionHaciaJugador    (tNodoArbolNario* actual, tEstado* estado);


#endif //CRIATURA_H_INCLUDED
