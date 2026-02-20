#include "Criatura.h"
//////////////////////////////////////////////////////////////////////////////
tNodoArbolNario* DeterminarDireccionHaciaJugador(tNodoArbolNario* actual, tEstado* estado)
{
    tNodoArbolNario* padre = NULL;
    tLista hijos;

    if (actual == estado->PosJugador) return NULL;

    if (EsAncestroPorId(actual, ((tInfoCamara*)estado->PosJugador->info)->id))
    {
        hijos = actual->hijos;
        while (hijos)
        {
            tNodoArbolNario* h = *(tNodoArbolNario**)hijos->Info;
            if (EsAncestroPorId(actual, ((tInfoCamara*)estado->PosJugador->info)->id))
                return h;

            hijos = hijos->sig;
        }
    }

    if (BuscarPadre(&(estado->MapaPadres), actual, &padre))
    {
        return padre;
    }

    return NULL;
}
int EsAncestroPorId(tNodoArbolNario* potencialAncestro, int idObjetivo)
{
    tCola cola;
    tNodoArbolNario* actual;
    tLista hijos;
    int encontrado = 0;

    if (((tInfoCamara*)potencialAncestro->info)->id == idObjetivo) return 1;

    CrearCola(&cola);
    PonerEnCola(&cola, &potencialAncestro, sizeof(tNodoArbolNario*));

    while (!ColaVacia(&cola) && !encontrado)
    {
        SacarDeCola(&cola, &actual, sizeof(tNodoArbolNario*));
        if (((tInfoCamara*)actual->info)->id == idObjetivo) encontrado = 1;

        hijos = actual->hijos;
        while (hijos && !encontrado)
        {
            tNodoArbolNario* h = *(tNodoArbolNario**)hijos->Info;
            PonerEnCola(&cola, &h, sizeof(tNodoArbolNario*));
            hijos = hijos->sig;
        }
    }
    VaciarCola(&cola);
    return encontrado;
}
void RecorrerBFS(tNodoArbolNario *Raiz, tAccionNodo Accion, void *Contexto)
{
    tCola Cola;
    tLista ListaHijos;
    tNodoArbolNario *Actual, *Hijo;

    if(!Raiz || !Accion) return;

    CrearCola(&Cola);
    PonerEnCola(&Cola, &Raiz, sizeof(tNodoArbolNario*));

    while(!ColaVacia(&Cola))
    {
        SacarDeCola(&Cola, &Actual, sizeof(tNodoArbolNario*));
        Accion(Actual, Contexto);

        ListaHijos = Actual->hijos;
        while(ListaHijos)
        {
            Hijo = *(tNodoArbolNario **)ListaHijos->Info;
            PonerEnCola(&Cola, &Hijo, sizeof(tNodoArbolNario*));
            ListaHijos = ListaHijos->sig;
        }
    }
    VaciarCola(&Cola);
}

void RecorrerDFSpreorden(const tNodoArbolNario *Nodo, tAccionNodo Accion, void *Contexto)
{
    tLista actual;
    tNodoArbolNario* hijo;

    if(!Nodo || !Accion) return;

    Accion((tNodoArbolNario*)Nodo, Contexto);
    actual = Nodo->hijos;

    while(actual)
    {
        hijo = *(tNodoArbolNario**)actual->Info;
        RecorrerDFSpreorden(hijo, Accion, Contexto);
        actual = actual->sig;
    }
}
//////////////////////////////////////////////////////////////////////////////
void MoverCriaturas(tNodoArbolNario* raiz, tEstado *estado)
{
    tInfoCamara* info;
    tNodoArbolNario* destino;
    tLista hijos;

    if (!raiz) return;

    info = (tInfoCamara*)raiz->info;

    if (info->cant_criaturas > 0 && raiz != estado->PosJugador)
    {
        destino = DeterminarDireccionHaciaJugador(raiz, estado);

        if (destino)
        {
            tInfoCamara* infoDestino = (tInfoCamara*)destino->info;
            info->cant_criaturas--;
            infoDestino->cant_criaturas++;
        }
    }
    hijos = raiz->hijos;
    while (hijos)
    {
        tNodoArbolNario* h = *(tNodoArbolNario**)hijos->Info;
        MoverCriaturas(h, estado);
        hijos = hijos->sig;
    }
}
