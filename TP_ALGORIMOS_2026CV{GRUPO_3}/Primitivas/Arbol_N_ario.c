#include "Arbol_N_ario.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CrearArbolNario(tArbolNario *p)
{
    *p = NULL;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VaciarArbolNario(tArbolNario *p)
{
    tNodoArbolNario* hijo;

    if(!*p)
        return;

    while(sacarPrimeroLista(&(*p)->hijos, &hijo, sizeof(tNodoArbolNario *)) == TODO_OK)
    {
        VaciarArbolNario(&hijo);
    }
    free((*p)->info);
    free(*p);
    *p = NULL;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CrearNodoArbolNario(tArbolNario *p, const void *Dato, unsigned Bytes)
{
    tNodoArbolNario *nue;

    nue = (tNodoArbolNario*)malloc(sizeof(tNodoArbolNario));
    if(!nue)
        return SIN_MEM;

    nue->info = malloc(Bytes);
    if(!nue->info)
    {
        free(nue);
        return SIN_MEM;
    }

    memcpy(nue->info, Dato, Bytes);
    nue->tamInfo = Bytes;

    nue->hijos = NULL;
    *p = nue;

    return TODO_OK;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int EsHoja(const tNodoArbolNario* Nodo)
{
    if(!Nodo)
        return ESTRUCTURA_INVALIDA;

    if(!Nodo->hijos)
        return ES_HOJA;

    return NO_HOJA;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int AgregarHijo(tNodoArbolNario *padre, const void *DatoHijo, unsigned Bytes, tNodoArbolNario **pHijo)
{
    tNodoArbolNario *nueHijo;
    int res;

    res = CrearNodoArbolNario(&nueHijo, DatoHijo, Bytes);
    if(res != TODO_OK)
        return res;

    res = PonerAlFinalEnLista(&(padre->hijos), &nueHijo, sizeof(tNodoArbolNario*));
    if(res != TODO_OK)
    {
        free(nueHijo->info);
        free(nueHijo);
        return res;
    }

    if(pHijo)
        *pHijo = nueHijo;

    return TODO_OK;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int HijoEnPos(const tNodoArbolNario* actual, int pos, tNodoArbolNario **destino)
{
    tLista listaHijos;
    int i = 0;

    if(!actual)
        return ESTRUCTURA_INVALIDA;

    if(!actual->hijos)
        return ARBOL_VACIO;

    listaHijos = actual->hijos;

    while (listaHijos && i < pos)
    {
        listaHijos = listaHijos->sig;
        i++;
    }

    if (listaHijos && listaHijos->Info)
    {
        *destino = *(tNodoArbolNario**)listaHijos->Info;
        return TODO_OK;
    }

    return NO_EXISTE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
tNodoArbolNario* ObtenerPrimeraHoja(tNodoArbolNario *p)
{
    if(!p)
        return NULL;

    tNodoArbolNario *aux = p;
    tNodoArbolNario *primerHijo;

    while(verPrimeroLista(&(aux->hijos), &primerHijo, sizeof(tNodoArbolNario*)) == TODO_OK)
    {
        aux = primerHijo;
    }

    return aux;
}
