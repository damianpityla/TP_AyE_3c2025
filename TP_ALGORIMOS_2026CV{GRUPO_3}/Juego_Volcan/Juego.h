#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED


#include <windows.h>
#include <conio.h>
#include <ctype.h>
#include "Juego.h"
#include "../Config/Configuracion.h"
#include "Volcan.h"
#include "Criatura.h"
#include "../Primitivas/Cola.h"
#include "../Primitivas/Arbol_N_ario.h"

#define MUSICA_FONDO "crash.wav" ///musica de fondo del Crash Bandicoot 1

void EjecutarCicloJuego(tEstado* estado, struct sConfig* config, const char *ArchConfiguraciones);
int  ProcesarEntradaJugador(tCola* colaMovimientos);
void ActualizarEstadoJuego(tEstado* estado, tCola* colaMovimientos, int* jugando);
void DibujarInterfaz(tEstado* estado, int nivelLavaActual, int alturaMaxima);
void MostrarMapaJerarquico(tNodoArbolNario* nodo, tEstado* estado, int nivel, int posPadre, int esUltimo);
void MostrarRegistroMovimientos(tEstado* estado);
#endif // JUEGO_H_INCLUDED

