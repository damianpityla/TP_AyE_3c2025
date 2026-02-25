#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED

/* ---         BIBLIOTECAS USADAS        --- */
#include <windows.h>
#include <conio.h>
#include <ctype.h>
#include <mmsystem.h>

#include "../Config/Configuracion.h"
#include "Criatura.h"

/* ---         MACROS DE SOLUCIONES         --- */
#define SIN_SOLUCION   0
#define HAY_SOLUCION   1

/* ---         MACROS DE COLORES         --- */
#define CLR_ROJO   (12 | FOREGROUND_INTENSITY)
#define CLR_AMARILLO (14 | FOREGROUND_INTENSITY)
#define CLR_VERDE  (10 | FOREGROUND_INTENSITY)
#define CLR_CYAN (11 | FOREGROUND_INTENSITY)
#define CLR_RESET  7

/* ---         MUSICA INCLUIDA         --- */
#define MUSICA_FONDO "crash.wav" ///musica de fondo del Crash Bandicoot 1

/* ---         ESTRUCTURAS USADAS        --- */
typedef enum { MOV_CRIATURA, MOV_LAVA } tTipoMovimiento;

typedef struct
{
    tTipoMovimiento tipo;
    int nivelLava;
} tEventoComputadora;


void            EjecutarCicloJuego              (tEstado *estado, tConfig *config, const char *ArchConfiguraciones);
int             ProcesarEntradaJugador          (tCola *colaMovimientos);
void            ActualizarEstadoJuego           (tEstado *estado, tCola *colaMovimientos, int *jugando);
void            DibujarInterfaz                 (tEstado *estado, int nivelLavaActual, int alturaMaxima);
void            MostrarMapaJerarquico           (tNodoArbolNario *nodo, tEstado *estado, int nivel, int posPadre, int esUltimo);
void            MostrarRegistroMovimientos      (tEstado *estado);
int             TieneSolucion                   (tNodoArbolNario *nodo);

#endif // JUEGO_H_INCLUDED

