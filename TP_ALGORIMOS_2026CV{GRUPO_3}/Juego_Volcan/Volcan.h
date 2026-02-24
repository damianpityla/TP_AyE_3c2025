#ifndef VOLCAN_H_INCLUDED
#define VOLCAN_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../Primitivas/Arbol_N_ario.h"

#define ARCH_VOLCAN "volcan.txt"

#define TAM_DESCRIPCION 50

#define JUGADOR  'J'
#define SALIDA   'S'
#define PREMIO   'P'
#define CRIATURA 'C'
#define VIDA     'V'
#define VACIO    '0'
#define LAVA     'L'

struct sConfig;
typedef struct sConfig tConfig;

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
    int cant_criaturas;
} tInfoCamara;

typedef struct {
    int idOrigen;
    int idDestino;
    char descripcion[TAM_DESCRIPCION + 1];
} tMovimientoLog;

typedef struct sEstado {
    tNodoArbolNario* Volcan;
    tNodoArbolNario* PosJugador;
    tLista MapaPadres;
    tLista Historial;
    int Vidas;
    int Puntaje;
    int TurnosJugados;
} tEstado;

typedef struct
{
    tNodoArbolNario *Hijo,
                    *Padre;
}
tParPadre;

int GenerarEstructuraVolcan(tEstado* estado, tConfig* config);
void CrearRamasAleatorias(tNodoArbolNario* padre, int nivelActual, tConfig* config, int* proximoId);
void CensoDeCamaras(tNodoArbolNario* raiz, tLista* listaPunteros);
tNodoArbolNario* SortearCamaraVacia(tLista* listaPunteros);
tNodoArbolNario* SortearCamaraCualquiera(tLista* listaPunteros);
void DibujarArbolEnArchivo(FILE* pf, tNodoArbolNario* nodo, const tEstado* estado, int nivel, int xCentro);
void GrabarArchivoVolcan(tNodoArbolNario* raiz, const tEstado* estado, const char* nombreArchivo);
void ActualizarMapaPadres(tNodoArbolNario* raiz, tLista* mapaPadres);
int ObtenerProfundidadCamara(const tLista* mapaPadres, tNodoArbolNario* nodo);
int BuscarPadre(const tLista *MapaPadres, tNodoArbolNario *Hijo, tNodoArbolNario **pPadre);

//otros archivos
void ConstruirMapaPadres(tNodoArbolNario *Raiz, tLista *pMapaPadres);
int Profundidad(const tLista *MapaPadres, tNodoArbolNario *Nodo);
int AgregarHijo(tNodoArbolNario *Padre, const void *InfoHijo, unsigned TamInfo, tNodoArbolNario **pHijo);
int CondEsHijo(const void *Elem, const void *Contexto);
void AvanzarLava(tNodoArbolNario* nodo, int nivelObjetivo, int nivelActual);
void PoblarCamaras(tEstado* estado, struct sConfig* config);


#endif // VOLCAN_H_INCLUDED
