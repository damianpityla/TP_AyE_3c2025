#include "Criatura.h"
//////////////////////////////////////////////////////////////////////////////
tNodoArbolNario *DeterminarDireccionHaciaJugador(tNodoArbolNario *actual, tEstado *estado)
{
    tNodoArbolNario *padre = NULL;
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
//////////////////////////////////////////////////////////////////////////////
int EsAncestro(tNodoArbolNario *potencialAncestro, tNodoArbolNario* objetivo)
{
    tLista hijos;

    if (!potencialAncestro || !objetivo)
        return NO_EXISTE;

    if (potencialAncestro == objetivo)
    return TODO_OK;

    hijos = potencialAncestro->hijos;
    while (hijos)
    {
        if (EsAncestro(*(tNodoArbolNario**)hijos->Info, objetivo))
            return TODO_OK;

        hijos = hijos->sig;
    }
    return NO_EXISTE;
}
//////////////////////////////////////////////////////////////////////////////
void VerificarCombate(tEstado* estado)
{
    tInfoCamara* info = (tInfoCamara*)estado->PosJugador->info;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int i;

    if (info->cant_criaturas > 0)
    {
        info->cant_criaturas = 0;
        estado->Vidas--;
        Beep(250, 600);

        if (estado->Vidas > 0)
        {
            system("cls");
            SetConsoleTextAttribute(hConsole, 12 | FOREGROUND_INTENSITY);
            printf("\n\t%c", 201);

            for(i = 0; i < 52; i++)
                printf("%c", 205);

            printf("%c\n", 187);

            printf("\t%c  [!] PELIGRO: HAS SIDO ALCANZADO POR UNA CRIATURA  %c\n", 186, 186);

            printf("\t%c", 204);

            for(i = 0; i < 52 ; i++)
                printf("%c", 205);

            printf("%c\n", 185);

            SetConsoleTextAttribute(hConsole, 7);
            printf("\t%c  Vidas restantes: %-26d       %c\n", 186, estado->Vidas, 186);
            printf("\t%c  Reapareciendo en la misma camara...               %c\n", 186, 186);

            printf("\t%c", 200);

            for(i = 0; i < 52 ; i++)
                printf("%c", 205);

            printf("%c\n", 188);

            SetConsoleTextAttribute(hConsole, 14 | FOREGROUND_INTENSITY);
            printf("\n\t>> Presione cualquier tecla para continuar la exploracion...");
            SetConsoleTextAttribute(hConsole, 7);
        }
    }
        else
        {
            printf("\n [!] Sin vidas restantes. El volcan reclama tu alma...\n");
        }

}
//////////////////////////////////////////////////////////////////////////////
void MoverCriaturas(tNodoArbolNario *raiz, tEstado *estado)
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
