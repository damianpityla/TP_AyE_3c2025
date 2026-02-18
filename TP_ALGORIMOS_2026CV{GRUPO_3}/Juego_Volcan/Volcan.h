#ifndef VOLCAN_H_INCLUDED
#define VOLCAN_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "Usuario.h"
#include "../Primitivas/Arbol_N_ario.h"
#include "Criatura.h"

#define ARCH_VOLCAN "volcan.txt"

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


typedef struct {
    int id;
    unsigned char es_salida;
    unsigned char hay_premio;
    unsigned char hay_vida;
    unsigned char hay_lava;
    int cant_criaturas; // Agregalo para el bucle de "config->criaturas"
} tInfoCamara;


typedef struct sEstado
{
    tArbolNario Volcan;
    tNodoArbolNario *PosJugador;
    int Vidas;
    int Puntaje;
    int TurnosJugados;
    tLista MapaPadres;
} tEstado;

typedef struct
{
    tNodoArbolNario *Hijo,
                    *Padre;
}
tParPadre;

int GenerarEstructuraVolcan(tEstado* estado, tConfig* config);
void CrearRamasAleatorias(tNodoArbolNario* padre, int nivelActual, struct sConfig* config, int* proximoId);
void CensoDeCamaras(tNodoArbolNario* raiz, tLista* listaPunteros);
tNodoArbolNario* SortearCamaraVacia(tLista* listaPunteros);
tNodoArbolNario* SortearCamaraCualquiera(tLista* listaPunteros);
void DibujarCamaraEnArchivo(FILE* pf, tNodoArbolNario* nodo, const struct sEstado* estado, int nivel, int* prefijo, int esUltimo);
void GrabarArchivoVolcan(tNodoArbolNario* raiz, const struct sEstado* estado, const char* nombreArchivo);
void ActualizarMapaPadres(tNodoArbolNario* raiz, tLista* mapaPadres);
int ObtenerProfundidadCamara(const tLista* mapaPadres, tNodoArbolNario* nodo);
int BuscarPadre(const tLista *MapaPadres, tNodoArbolNario *Hijo, tNodoArbolNario **pPadre);

// Estas vienen de otros archivos, asegurate de incluir sus .h o declararlas:
void ConstruirMapaPadres(tNodoArbolNario *Raiz, tLista *pMapaPadres);
int Profundidad(const tLista *MapaPadres, tNodoArbolNario *Nodo);
int AgregarHijo(tNodoArbolNario *Padre, const void *InfoHijo, unsigned TamInfo, tNodoArbolNario **pHijo);
int CondEsHijo(const void *Elem, const void *Contexto);
void AvanzarLava(tNodoArbolNario* nodo, int nivelLava, const tLista* mapaPadres);


#endif // VOLCAN_H_INCLUDED
