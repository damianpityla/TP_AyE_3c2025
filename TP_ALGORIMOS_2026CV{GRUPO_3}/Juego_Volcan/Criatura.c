#include "Criatura.h"
//////////////////////////////////////////////////////////////////////////////
tNodoArbolNario *DeterminarDireccionHaciaJugador(tNodoArbolNario* actual, tEstado* estado)
{
    tNodoArbolNario *padre = NULL;;
    tNodoArbolNario *h;
    tLista hijos;

    if (actual == estado->PosJugador)
        return NULL;

    if (EsAncestro(actual, estado->PosJugador))
    {
        hijos = actual->hijos;
        while (hijos)
        {
            h = *(tNodoArbolNario**)hijos->Info;
            if(EsAncestro(h, estado->PosJugador))
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
int EsAncestro(tNodoArbolNario* potencialAncestro, tNodoArbolNario* objetivo)
{
    tLista hijos;

    if (!potencialAncestro || !objetivo) return 0;
    if (potencialAncestro == objetivo) return 1;

    hijos = potencialAncestro->hijos;
    while (hijos)
    {
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
        Beep(250, 400);

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

    if(!Nodo || !Accion)
        return;

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
    tCola ColaExploracion;
    tCola ColaCambios;
    tInfoCamara *infoActual;
    tNodoArbolNario *destino;
    tNodoArbolNario *h;
    tNodoArbolNario *actual;
    tMovimientoCriatura mov;
    tLista hijos;

    CrearCola(&ColaExploracion);
    CrearCola(&ColaCambios);

    PonerEnCola(&ColaExploracion, &raiz, sizeof(tNodoArbolNario*));

    while(!ColaVacia(&ColaExploracion))
    {
        SacarDeCola(&ColaExploracion, &actual, sizeof(tNodoArbolNario*));
        infoActual = (tInfoCamara*)actual->info;

        hijos = actual->hijos;
        while (hijos)
        {
            h = *(tNodoArbolNario**)hijos->Info;
            PonerEnCola(&ColaExploracion, &h, sizeof(tNodoArbolNario*));
            hijos = hijos->sig;
        }

        if(infoActual->cant_criaturas > 0 && actual != estado->PosJugador)
        {
            destino = DeterminarDireccionHaciaJugador(actual, estado);
            if (destino && destino != actual)
            {
                mov.origen = infoActual;
                mov.destino = (tInfoCamara*)destino->info;
                mov.cantidad = infoActual->cant_criaturas;

                PonerEnCola(&ColaCambios, &mov, sizeof(tMovimientoCriatura));
            }
        }
    }
    while(!ColaVacia(&ColaCambios))
    {
        SacarDeCola(&ColaCambios, &mov, sizeof(tMovimientoCriatura));
        mov.origen->cant_criaturas -= mov.cantidad;
        mov.destino->cant_criaturas += mov.cantidad;
    }

    VaciarCola(&ColaExploracion);
    VaciarCola(&ColaCambios);
}
