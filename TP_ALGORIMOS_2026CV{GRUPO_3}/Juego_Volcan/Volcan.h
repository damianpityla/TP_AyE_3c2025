#ifndef VOLCAN_H_INCLUDED
#define VOLCAN_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "Usuario.h"


#define JUGADOR  'J'
#define SALIDA   'S'
#define PREMIO   'P'
#define CRIATURA 'C'
#define VIDA     'V'
#define VACIO    '0'
#define LAVA     'L'

typedef struct{
    int turno;
    int NombreJugador;
    int Origen;
    int Destino;
    //char accion[20];
    //char resultado[30];
}sMovimiento;

#endif // VOLCAN_H_INCLUDED
