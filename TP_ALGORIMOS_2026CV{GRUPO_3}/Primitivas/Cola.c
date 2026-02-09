#include "Cola.h"
void crearCola(tCola* pc)
{
    pc->pri = NULL;
    pc->ult = NULL;
}
int colaLlena(const tCola* pc, unsigned tam)
{
    tNodoCola* aux = (tNodoCola*) malloc(sizeof(tNodoCola));
    void* info = malloc(tam);
    free(aux);
    free(info);
    return aux==NULL || info==NULL;
}
int ponerEnCola(tCola* pc, const void* pd, unsigned tam)
{
    tNodoCola* nue = (tNodoCola*) malloc(sizeof(tNodoCola));
    if(nue==NULL || (nue->info = malloc(tam))==NULL)
    {
        free(nue);
        return 0;
    }
    memcpy(nue->info, pd, tam);
    nue->tamInfo = tam;
    nue->sig = NULL;
    if(pc->ult)
        pc->ult->sig = nue;
    else
        pc->pri = nue;
    pc->ult = nue;
    return 1;
}
int verPrimeroCola(const tCola* pc, void* pd, unsigned tam)
{
    if(pc->pri == NULL)
        return 0;
    memcpy(pd,pc->pri->info, MINIMO(tam,pc->pri->tamInfo));
    return 1;
}
int colaVacia(const tCola* pc)
{
    return pc->pri == NULL;
}
int sacarDeCola(tCola* pc, void* pd, unsigned tam)
{
    tNodoCola* aux = pc->pri;
    if(aux== NULL)
        return 0;
    pc->pri = aux->sig;
    memcpy(pd, aux->info, MINIMO(aux->tamInfo, tam));
    free(aux->info);
    free(aux);
    if(pc->pri == NULL)
        pc->ult = NULL;
    return 1;
}
void vaciarCola(tCola* pc)
{
    while(pc->pri)
    {
        tNodoCola* aux = pc->pri;
        pc->pri = aux->sig;
        free(aux->info);
        free(aux);
    }
    pc->ult = NULL;
}
