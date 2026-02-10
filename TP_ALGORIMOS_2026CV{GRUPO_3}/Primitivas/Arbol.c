#include "Arbol.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CrearArbol(tArbol* pa)
{
    *pa = NULL;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VaciarArbol(tArbol* pa)
{
    if(!*pa)
        return;
    tNodoArbol* hijo;

    while(!sacarPrimeroLista(&(*pa)->hijos,&hijo,sizeof(tNodoArbol)))
    {
        VaciarArbol(&hijo);
    }
    free((*pa)->info);
    free(*pa);
    *pa = NULL;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CrearNodoArbol(tNodoArbol** p, const void* pd, unsigned tam)
{
    if(*p != NULL)
        return 0;
    tNodoArbol* nue = (tNodoArbol*)malloc(sizeof(tNodoArbol));
    if(!nue)
        return 0;

    nue->info = malloc(tam);
    if(!nue->info)
    {
        free(nue);
        return 0;
    }
    memcpy(nue->info,pd, tam);
    nue->tamInfo = tam;
    CrearLista(&nue->hijos);
    *p = nue;
    return TODO_OK;
}













/*
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CrearArbolBin(tArbolBin* p)
{
    *p = NULL;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VaciarArbolBin(tArbolBin* p)
{
    if(*p)
    {
        VaciarArbolBin(&(*p)->hijoIzq);
        VaciarArbolBin(&(*p)->hijoDer);
        free((*p)->info);
        free(*p);
        *p = NULL;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int InsertarEnArbolRecursivo(tArbolBin *p, const void* Dato, unsigned Bytes, tCmp cmp)
{
    tNodoArbolBin* nue;
    int resCmp;

    if(*p == NULL)
    {
        nue = (tNodoArbolBin*)malloc(sizeof(tNodoArbolBin));
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
        nue->hijoDer = NULL;
        nue->hijoIzq = NULL;
        *p = nue;
        return TODO_OK;
    }

    resCmp = cmp((*p)->info, Dato);

    if(resCmp > 0)
        return InsertarEnArbolRecursivo(&(*p)->hijoIzq, Dato, Bytes, cmp);

    else if(resCmp < 0)
        return InsertarEnArbolRecursivo(&(*p)->hijoDer, Dato, Bytes, cmp);

    return DUPLICADO;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
tArbolBin* BusquedaEnArbolRecursivo(tArbolBin *p, const void* Dato, tCmp cmp)
{
    int resCmp;

    if(!(*p))
        return NULL;

    resCmp = cmp((*p)->info, Dato);

    if(resCmp > 0)
        return BusquedaEnArbolRecursivo(&(*p)->hijoIzq, Dato, cmp);

    else if(resCmp < 0)
        return BusquedaEnArbolRecursivo(&(*p)->hijoDer, Dato, cmp);

    return p;
}*/

