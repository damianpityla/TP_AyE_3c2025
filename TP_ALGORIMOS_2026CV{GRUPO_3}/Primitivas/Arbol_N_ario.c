#include "Arbol_N_ario.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CrearArbolNario(tArbolNario *p)
{
    *p = NULL;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VaciarArbolNario(tArbolNario *p)
{
    if(!*p)
        return;

    tNodoArbolNario* hijo;

    while(sacarPrimeroLista(&(*p)->hijos, &hijo, sizeof(tNodoArbolNario *)))
    {
        VaciarArbolNario(&hijo);
    }
    free((*p)->info);
    free(*p);
    *p = NULL;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CrearNodoArbolNario(tNodoArbolNario **pNodo, const void *info, unsigned tam)
{
    *pNodo = (tNodoArbolNario*)malloc(sizeof(tNodoArbolNario));
    if (!(*pNodo)) return 0;

    (*pNodo)->info = malloc(tam);
    if (!((*pNodo)->info))
    {
        free(*pNodo);
        return 0;
    }

    memcpy((*pNodo)->info, info, tam);

    // ESTA LÍNEA ES LA QUE FALTA:
    (*pNodo)->hijos = NULL;

    return 1;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int EsHoja(const tNodoArbolNario* Nodo)
{
    int esHoja = 0;

    if(Nodo)
    {
        if(Nodo->hijos == NULL)
        {
            esHoja = 1;
        }
    }

    return esHoja;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///agregar un hijo y guardar la lista de hijos en un puntero al nodo hijo
int AgregarHijo(tNodoArbolNario *padre, const void *infoHijo, unsigned tam, tNodoArbolNario **pHijo)
{
    tNodoArbolNario *nuevoHijo;

    nuevoHijo = (tNodoArbolNario*)malloc(sizeof(tNodoArbolNario));
    if (!nuevoHijo) return 0;

    nuevoHijo->info = malloc(tam);
    if (!nuevoHijo->info)
    {
        free(nuevoHijo);
        return 0;
    }

    memcpy(nuevoHijo->info, infoHijo, tam);
    nuevoHijo->hijos = NULL;

    if (PonerAlFinalEnLista(&(padre->hijos), &nuevoHijo, sizeof(tNodoArbolNario*)))
    {
        if (pHijo) *pHijo = nuevoHijo;
        return 1;
    }

    free(nuevoHijo->info);
    free(nuevoHijo);
    return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CantHijos(const tNodoArbolNario *Padre)
{
    int Cant = 0;
    tLista actual;

    actual = Padre ? Padre->hijos : NULL;

    while(actual)
    {
        Cant++;
        actual = actual->sig;
    }

    return Cant;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// esto es como querer encontrar V[i]
int HijoEnPos(const tNodoArbolNario* actual, int pos, tNodoArbolNario** destino)
{
    tLista listaHijos;
    int i = 0;

    if (!actual || !actual->hijos)
        return 0;

    listaHijos = actual->hijos;

    while (listaHijos && i < pos)
    {
        listaHijos = listaHijos->sig;
        i++;
    }

    if (listaHijos && listaHijos->Info)
    {
        *destino = *(tNodoArbolNario**)listaHijos->Info;
        return 1;
    }

    return 0;
}
