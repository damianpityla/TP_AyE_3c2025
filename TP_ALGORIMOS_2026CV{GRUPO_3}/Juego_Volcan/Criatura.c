#include "Criatura.h"
//////////////////////////////////////////////////////////////////////////////
tNodoArbolNario* DeterminarDireccionHaciaJugador(tNodoArbolNario* actual, tEstado* estado)
{
    tNodoArbolNario* padre;
    tLista hijos;

    if (actual == estado->PosJugador) return NULL;

    if (EsAncestro(actual, estado->PosJugador))
    {
        hijos = actual->hijos;
        while (hijos)
        {
            tNodoArbolNario* h = *(tNodoArbolNario**)hijos->Info;
            if (EsAncestro(h, estado->PosJugador)) return h;
            hijos = hijos->sig;
        }
    }

    if (BuscarPadre(&(estado->MapaPadres), actual, &padre))
    {
        return padre;
    }

    return NULL;
}
int EsAncestro(tNodoArbolNario* potencialAncestro, tNodoArbolNario* objetivo)
{
    tLista hijos;

    if (!potencialAncestro || !objetivo) return 0;
    if (potencialAncestro == objetivo) return 1;

    hijos = potencialAncestro->hijos;
    while (hijos)
    {
        /* Casteo de Info a puntero de nodo para la recursion */
        if (EsAncestro(*(tNodoArbolNario**)hijos->Info, objetivo)) return 1;
        hijos = hijos->sig;
    }
    return 0;
}
void VerificarCombate(tEstado* estado)
{
    tInfoCamara* info;
    info = (tInfoCamara*)estado->PosJugador->info;

    if (info->cant_criaturas > 0)
    {
        info->cant_criaturas = 0;
        estado->Vidas--;

        if (estado->Vidas > 0)
        {
            printf("\n¡FUISTE ALCANZADO POR UNA CRIATURA! Pierdes una vida.\n");
            printf("Reapareces en la misma camara...\n");
            system("pause");
        }
    }
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
