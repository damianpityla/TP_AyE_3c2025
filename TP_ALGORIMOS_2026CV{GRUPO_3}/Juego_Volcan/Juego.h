#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED

#include "Volcan.h"

void EjecutarCicloJuego(tEstado* estado, tConfig* config);
int  ProcesarEntradaJugador(tCola* colaMovimientos);
void ActualizarEstadoJuego(tEstado* estado, tCola* colaMovimientos, int* jugando);
void DibujarInterfaz(const tEstado* estado);
void MostrarMapaJerarquico(tNodoArbolNario* nodo, const tEstado* estado, int nivel, int* prefijo, int esUltimo);
#endif // JUEGO_H_INCLUDED
