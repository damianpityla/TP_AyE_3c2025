#include "Lista.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CrearLista(tLista* p)
{
    *p = NULL;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VaciarLista(tLista* p)
{
    tNodoLista *elim;
    while(*p)
    {
        elim = *p;
        *p = elim->sig;
        free(elim->Info);
        free(elim);
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int sacarPrimeroLista(tLista *p, void *Dato, unsigned Bytes)
{
    tNodoLista *elim = *p;
    if(!elim)
        return VACIA;

    *p = elim->sig;

    memcpy(Dato, elim->Info, MINIMO(Bytes, elim->TamInfo));

    free(elim->Info);
    free(elim);

    return TODO_OK;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RecorrerLista(const tLista* p, tAccion Accion)
{
    while(*p)
    {
        Accion((*p)->Info);
        p = &(*p)->sig;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int InsertarEnOrdenLista(tLista* p, const void *Dato, unsigned Bytes, int Dup, tCmp Cmp, tAcum Acum)
{
    tNodoLista* nue;

    while((*p) && (Cmp(Dato, (*p)->Info) > 0))
        p = &(*p)->sig;

    if((*p) && Cmp(Dato, (*p)->Info) == 0)
    {
        if(Dup == SIN_DUPLICADOS)
        {
            if(Acum)
                Acum((*p)->Info, Dato);
            return DUPLICADO;
        }
    }

    nue = (tNodoLista*)malloc(sizeof(tNodoLista));
    if(!nue)
        return SIN_MEM;

    nue->Info = malloc(Bytes);
    if(!nue->Info)
    {
        free(nue);
        return SIN_MEM;
    }

    memcpy(nue->Info, Dato, Bytes);
    nue->TamInfo = Bytes;

    nue->sig = *p;
    *p = nue;

    return TODO_OK;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int PonerAlFinalEnLista(tLista *p, const void *Dato, unsigned Bytes)
{
    tNodoLista *nue;

    while(*p)
        p = &(*p)->sig;

    if(!(nue = (tNodoLista *)malloc(sizeof(tNodoLista))))
        return SIN_MEM;

    if(!(nue->Info = malloc(Bytes)))
    {
        free(nue);
        return SIN_MEM;
    }

    memcpy(nue->Info, Dato, Bytes);
    nue->TamInfo = Bytes;

    nue->sig = *p;
    *p = nue;

    return TODO_OK;
}
///esta es como strchr
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int BuscarPrimeroCondicionLista(tLista* p, void* Dato, unsigned Bytes, tCondicion Condicion, const void *Contexto)
{
    while(*p)
    {
        if(Condicion((*p)->Info, Contexto))
        {
            memcpy(Dato, (*p)->Info, MINIMO(Bytes, (*p)->TamInfo));
            return 1;
        }
        p = &(*p)->sig;
    }
    return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int verPrimeroLista(tLista *p, void *Dato, unsigned Bytes)
{
    if(!*p)
        return VACIA;

    memcpy(Dato, (*p)->Info, MINIMO(Bytes, (*p)->TamInfo));

    return TODO_OK;
}
