#include "Arbol_Binario.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CrearArbolBin(tArbolBin *p)
{
    *p = NULL;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int InsertarEnArbolBin(tArbolBin* p, const void *Dato, unsigned Bytes, tCmp cmp)
{
    tNodoArbolBin *nue;
    int rc;

    if(*p)
    {
        rc = cmp(Dato, (*p)->Info);
        if(rc < 0)
            return InsertarEnArbolBin(&(*p)->Izq, Dato, Bytes, cmp);

        if(rc > 0)
            return InsertarEnArbolBin(&(*p)->Der, Dato, Bytes, cmp);

        return DUPLICADO;
    }

    if(!(nue = (tNodoArbolBin *)malloc(sizeof(tNodoArbolBin))))
        return SIN_MEM;

    if(!(nue->Info = malloc(Bytes)))
    {
        free(nue);
        return SIN_MEM;
    }
    memcpy(nue->Info, Dato, Bytes);
    nue->TamInfo = Bytes;

    nue->Izq = NULL;
    nue->Der = NULL;

    *p = nue;

    return TODO_OK;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
tArbolBin* BusquedaEnArbolBin(tArbolBin* p, const void *Dato, tCmp cmp)
{
    int rc;
    if(!*p)
        return NULL;

    rc = cmp(Dato, (*p)->Info);

    if(rc < 0)
    {
        return BusquedaEnArbolBin(&(*p)->Izq, Dato, cmp);
    }
    else if(rc > 0)
    {
        return BusquedaEnArbolBin(&(*p)->Der, Dato, cmp);
    }
    return p;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VaciarArbolBin(tArbolBin* p)
{
    if(!*p)
        return;

    VaciarArbolBin(&(*p)->Izq);
    VaciarArbolBin(&(*p)->Der);
    free((*p)->Info);
    free(*p);

    *p = NULL;
}
