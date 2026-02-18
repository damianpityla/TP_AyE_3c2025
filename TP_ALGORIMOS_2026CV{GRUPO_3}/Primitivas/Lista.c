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
int sacarPrimeroLista(tLista* pl, void* pd, unsigned tam)
{
    tNodoLista* aux = *pl;
    if(aux == NULL)
        return 0;
    *pl = aux->sig;
    memcpy(pd, aux->Info, MINIMO(tam,aux->TamInfo));
    free(aux->Info);
    free(aux);
    return 1;
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
tNodoLista* BuscarAnteriorDelMenor(tLista* p, tCmp cmp)
{
    tNodoLista *anteriorMenor = NULL;
    tNodoLista *anterior = NULL;
    tNodoLista *actual;
    tNodoLista *menor;

    if(!*p || !(*p)->sig)
        return NULL;

    menor = *p;
    actual = (*p)->sig;
    anterior = *p;

    while(actual)
    {
        if(cmp(menor->Info, actual->Info) > 0)
        {
            menor = actual;
            anteriorMenor = anterior;
        }

        anterior = actual;
        actual = actual->sig;
    }

    return anteriorMenor;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int OrdenarLista(tLista* p, tCmp cmp)
{
    tNodoLista* anteriorMenor;
    tNodoLista* nodoDesenganchado;

    if(!*p)
        return TODO_OK;

    while(*p && (*p)->sig)
    {
        anteriorMenor = BuscarAnteriorDelMenor(p, cmp);

        if(anteriorMenor == NULL)
        {
            p = &(*p)->sig;
        }
        else
        {
            nodoDesenganchado = anteriorMenor->sig;
            anteriorMenor->sig = nodoDesenganchado->sig;

            nodoDesenganchado->sig = *p;
            *p = nodoDesenganchado;

            p = &(*p)->sig;
        }
    }

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
int BuscarPrimeroCondicionLista(const tLista* p, void* Dato, unsigned Bytes, tCondicion Condicion, const void *Contexto)
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
