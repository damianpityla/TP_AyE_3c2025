#include "Cola.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CrearCola(tCola* pc)
{
    pc->pri = NULL;
    pc->ult = NULL;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int PonerEnCola(tCola *p, const void *Dato, unsigned Bytes)
{
    tNodoCola *nue;

    if(!(nue = (tNodoCola *)malloc(sizeof(tNodoCola))))
        return SIN_MEM;

    if(!(nue->info = malloc(Bytes)))
    {
        free(nue);
        return SIN_MEM;
    }
    memcpy(nue->info, Dato, Bytes);
    nue->tamInfo = Bytes;
    nue->sig = NULL;

    if(p->ult)
        p->ult->sig = nue;
    else
        p->pri = nue;

    p->ult = nue;


    return TODO_OK;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int ColaVacia(tCola* pc)
{
    return pc->pri == NULL;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int SacarDeCola(tCola *p, void *Dato, unsigned Bytes)
{
    tNodoCola *elim = p->pri;
    if(!elim)
        return VACIA;

    p->pri = elim->sig;

    memcpy(Dato, elim->info, MIN(Bytes, elim->tamInfo));

    free(elim->info);
    free(elim);

    if(!(p->pri))
        p->ult = NULL;

    return TODO_OK;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VaciarCola(tCola *p)
{
    tNodoCola *elim;
    while(p->pri)
    {
        elim = p->pri;
        p->pri = elim->sig;

        free(elim->info);
        free(elim);
    }
    p->ult = NULL;
}

