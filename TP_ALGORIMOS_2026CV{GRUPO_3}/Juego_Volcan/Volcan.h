#ifndef VOLCAN_H_INCLUDED
#define VOLCAN_H_INCLUDED

#include "../Primitivas/Arbol_N_ario.h"
#include "../Config/Configuracion.h"

#define ARCH_VOLCAN "volcan.txt"
#define VOLCAN_ERROR 0
#define TAM_DESCRIPCION 50


typedef struct sConfig tConfig;

typedef struct
{
    int id;
    unsigned char es_salida;
    unsigned char hay_premio;
    unsigned char hay_vida;
    unsigned char hay_lava;
    int cant_criaturas;
}tInfoCamara;

typedef struct
{
    tInfoCamara *origen;
    tInfoCamara *destino;
    int cantidad;
} tMovimientoCriatura;

typedef struct
{
    int idOrigen;
    int idDestino;
    char descripcion[TAM_DESCRIPCION + 1];
}tMovimientoLog;

typedef struct sEstado
{
    tNodoArbolNario *Volcan;
    tNodoArbolNario *PosJugador;
    tLista MapaPadres;
    tLista Historial;
    int Vidas;
    int Puntaje;
    int TurnosJugados;
}tEstado;

typedef struct
{
    tNodoArbolNario *Hijo,
                    *Padre;
}
tParPadre;

int              GenerarEstructuraVolcan        (tEstado* estado, tConfig *config);
void             CrearRamasAleatorias           (tNodoArbolNario* padre, int nivelActual, tConfig *config, int *proximoId);
void             CensoDeCamaras                 (tNodoArbolNario* raiz, tLista* listaPunteros);
tNodoArbolNario *SortearCamaraVacia             (tLista *listaPunteros);
tNodoArbolNario *SortearCamaraCualquiera        (tLista *listaPunteros);
void             DibujarArbolEnArchivo          (FILE* pf, tNodoArbolNario* nodo, const tEstado *estado, int nivel, int xCentro);
void             GrabarArchivoVolcan            (tNodoArbolNario* raiz, const tEstado* estado, const char* nombreArchivo);
int              BuscarPadre                    (tLista *MapaPadres, tNodoArbolNario *Hijo, tNodoArbolNario **pPadre);
void             ConstruirMapaPadres            (tNodoArbolNario *Raiz, tLista *pMapaPadres);
int              Profundidad                    (tLista *MapaPadres, tNodoArbolNario *Nodo);
int              CondEsHijo                     (const void *Elem, const void *Contexto);
void             PoblarCamaras                  (tEstado *estado, tConfig *config);


#endif // VOLCAN_H_INCLUDED
