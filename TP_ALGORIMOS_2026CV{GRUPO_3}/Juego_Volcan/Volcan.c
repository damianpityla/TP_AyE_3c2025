#include "Volcan.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int GenerarEstructuraVolcan(tEstado* estado, tConfig* config)
{
    tInfoCamara infoRaiz;
    int camarasCreadas = 0;

    infoRaiz.id = camarasCreadas++;
    infoRaiz.es_salida = 1;
    infoRaiz.hay_premio = 0;
    infoRaiz.hay_vida = 0;
    infoRaiz.hay_lava = 0;
    infoRaiz.cant_criaturas = 0;

    estado->Volcan = (tNodoArbolNario*)malloc(sizeof(tNodoArbolNario));
    if(!estado->Volcan)
        return VOLCAN_ERROR;

    estado->Volcan->info = malloc(sizeof(tInfoCamara));
    if(!estado->Volcan->info)
    {
        free(estado->Volcan);
        return VOLCAN_ERROR;
    }

    memcpy(estado->Volcan->info, &infoRaiz, sizeof(tInfoCamara));
    estado->Volcan->hijos = NULL;

    CrearRamasAleatorias(estado->Volcan, 0, config, &camarasCreadas);

    return TODO_OK;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CrearRamasAleatorias(tNodoArbolNario* padre, int nivelActual, tConfig* config, int* proximoId)
{
    int i;
    int cantidadHijos;
    tInfoCamara nuevaInfo;
    tNodoArbolNario* nodoHijoCreado;

    if (nivelActual >= config->altura_maxima) return;

    cantidadHijos = rand() % (config->max_conexiones + 1);

    for (i = 0; i < cantidadHijos; i++)
    {
        nuevaInfo.id = *proximoId;
        (*proximoId)++;

        nuevaInfo.hay_lava = 0;
        nuevaInfo.es_salida = 0;
        nuevaInfo.hay_premio = 0;
        nuevaInfo.hay_vida = 0;
        nuevaInfo.cant_criaturas = 0;

        if(AgregarHijo(padre, &nuevaInfo, sizeof(tInfoCamara), &nodoHijoCreado))
        {
            CrearRamasAleatorias(nodoHijoCreado, nivelActual + 1, config, proximoId);
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PoblarCamaras(tEstado* estado, tConfig *config)
{
    tLista listaAux;
    tNodoArbolNario *camara = NULL;
    tNodoArbolNario *nodoRaiz = NULL;
    int i;

    CrearLista(&listaAux);

    if(!estado->Volcan)
        return;

    CensoDeCamaras(estado->Volcan, &listaAux);

    if(listaAux)
        sacarPrimeroLista(&listaAux, &nodoRaiz, sizeof(tNodoArbolNario*));

    ///jugador
    estado->PosJugador = ObtenerPrimeraHoja(estado->Volcan);
    if(!estado->PosJugador)
    {
        VaciarLista(&listaAux);
        return;
    }
    ///criaturas
    for(i = 0; i < config->criaturas; i++)
    {
        camara = SortearCamaraCualquiera(&listaAux);
        if(!camara)
            break;

        ((tInfoCamara*)camara->info)->cant_criaturas++;
    }
    ///vidas
    for(i = 0; i < config->vidas_extra; i++)
    {
        camara = SortearCamaraVacia(&listaAux);
        if(!camara)
            break;

        ((tInfoCamara*)camara->info)->hay_vida = 1;
    }
    ///premios
    for(i = 0; i < config->premios; i++)
    {
        camara = SortearCamaraVacia(&listaAux);
        if (!camara)
            break;

        ((tInfoCamara*)camara->info)->hay_premio = 1;

    }

    VaciarLista(&listaAux);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CensoDeCamaras(tNodoArbolNario* nodo, tLista* listaCamaras)
{
    tLista hijos;
    tNodoArbolNario* h;

    if(!nodo)
        return;

    PonerAlFinalEnLista(listaCamaras, &nodo, sizeof(tNodoArbolNario*));

    hijos = nodo->hijos;
    while (hijos)
    {
        h = *(tNodoArbolNario**)hijos->Info;
        CensoDeCamaras(h, listaCamaras);
        hijos = hijos->sig;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
tNodoArbolNario* SortearCamaraCualquiera(tLista *ListaCenso)
{
    tLista Actual;
    int cant = 0;
    int PosicionSorteada;
    int i;
    tNodoArbolNario *NodoElegido = NULL;

    if (!ListaCenso || !(*ListaCenso))
        return NULL;

    Actual = *ListaCenso;
    while(Actual)
    {
        cant++;
        Actual = Actual->sig;
    }

    PosicionSorteada = rand() % cant;
    Actual = *ListaCenso;

    for (i = 0; i < PosicionSorteada; i++)
        Actual = Actual->sig;

    NodoElegido = *(tNodoArbolNario**)Actual->Info;

    return NodoElegido;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
tNodoArbolNario* SortearCamaraVacia(tLista* ListaCenso)
{
    tLista *Actual;
    tNodoLista *elim;
    int cant = 0, pos, i;
    tNodoArbolNario* NodoExtraido = NULL;

    Actual = ListaCenso;
    while(*Actual)
    {
        cant++;
        Actual = &((*Actual)->sig);
    }

    if(!cant)
        return NULL;

    pos = rand() % cant;
    Actual = ListaCenso;

    for (i = 0; i < pos; i++)
        Actual = &((*Actual)->sig);

    elim = *Actual;
    NodoExtraido = *(tNodoArbolNario**)elim->Info;

    *Actual = elim->sig;
    free(elim->Info);
    free(elim);

    return NodoExtraido;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GrabarArchivoVolcan(tNodoArbolNario* raiz, const tEstado* estado, const char *NombreArchivo)
{
    FILE* pVolcan;

    pVolcan = fopen(NombreArchivo, "wt");
    if(!pVolcan)
        return;

    DibujarArbolEnArchivo(pVolcan, raiz, estado, 0, 75);

    fclose(pVolcan);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DibujarArbolEnArchivo(FILE* pf, tNodoArbolNario* nodo, const tEstado* estado, int nivel, int coordenadaX)
{
    tNodoArbolNario *HijoActual;
    tInfoCamara *info;
    tLista ListaHijos;
    tLista aux;
    int espacio, conector, separador;
    int cantidadHijos;
    int nuevaX;
    int anchoAbanico;
    int SeparacionHorizontal;

    if (!nodo || !pf)
        return;

    SeparacionHorizontal = (nivel == 0) ? 42 : (nivel == 1) ? 20 : (nivel == 2) ? 10 : 5;

    info = (tInfoCamara*)nodo->info;

    for(espacio = 0; espacio < coordenadaX; espacio++)
        fprintf(pf, " ");

    if(nivel == 0)
        fprintf(pf, "S");
    else
        fprintf(pf, "O");

    if(nodo == estado->PosJugador)
        fprintf(pf, "[J]");

    if (info->cant_criaturas > 0 || info->hay_premio || info->hay_lava || info->hay_vida)
    {
        fprintf(pf, "[");
        if(info->cant_criaturas > 0)
            fprintf(pf, "C");
        if(info->hay_premio)
            fprintf(pf, "P");
        if(info->hay_vida)
            fprintf(pf, "V");
        if(info->hay_lava)
            fprintf(pf, "L");
        fprintf(pf, "]");
    }
    fprintf(pf, "\n");

    ListaHijos = nodo->hijos;
    if(ListaHijos)
    {
        cantidadHijos = 0;
        aux = ListaHijos;
        while(aux)
        {
            cantidadHijos++;
            aux = aux->sig;
        }

        anchoAbanico = (cantidadHijos - 1) * SeparacionHorizontal;
        nuevaX = coordenadaX - (anchoAbanico / 2);

        for(espacio = 0; espacio < coordenadaX; espacio++)
        {
            fprintf(pf, " ");
        }
        fprintf(pf, "|\n");

        if (cantidadHijos > 1)
        {
            for(espacio = 0; espacio < nuevaX; espacio++)
                fprintf(pf, " ");

            for(conector = 0; conector <= anchoAbanico; conector++)
                fprintf(pf, "-");

            fprintf(pf, "\n");

            for(espacio = 0; espacio < nuevaX; espacio++)
                fprintf(pf, " ");

            for(conector = 0; conector < cantidadHijos; conector++)
            {
                fprintf(pf, "+");
                if(conector < cantidadHijos - 1)
                {
                    for(separador = 0; separador < SeparacionHorizontal - 1; separador++)
                        fprintf(pf, " ");
                }
            }
            fprintf(pf, "\n");
            for(espacio = 0; espacio < nuevaX; espacio++)
                fprintf(pf, " ");

            for(conector = 0; conector < cantidadHijos; conector++)
            {
                fprintf(pf, "|");
                if(conector < cantidadHijos - 1)
                {
                    for(separador = 0; separador < SeparacionHorizontal - 1; separador++)
                        fprintf(pf, " ");
                }
            }
            fprintf(pf, "\n");
        }
        else
        {
            for(espacio = 0; espacio < coordenadaX; espacio++)
                fprintf(pf, " ");
            fprintf(pf, "|\n");
        }
        while(ListaHijos)
        {
            HijoActual = *(tNodoArbolNario**)ListaHijos->Info;
            DibujarArbolEnArchivo(pf, HijoActual, estado, nivel + 1, nuevaX);
            nuevaX += SeparacionHorizontal;
            ListaHijos = ListaHijos->sig;
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ConstruirMapaPadres(tNodoArbolNario *Raiz, tLista *pMapaPadres)
{
    tLista hijos;
    tNodoArbolNario *hijoActual;
    tParPadre par;

    if(!Raiz)
        return;

    hijos = Raiz->hijos;

    while (hijos)
    {
        hijoActual = *(tNodoArbolNario**)hijos->Info;

        if (hijoActual)
        {
            par.Hijo = hijoActual;
            par.Padre = Raiz;

            PonerAlFinalEnLista(pMapaPadres, &par, sizeof(tParPadre));

            ConstruirMapaPadres(hijoActual, pMapaPadres);
        }
        hijos = hijos->sig;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int BuscarPadre(tLista *MapaPadres, tNodoArbolNario *Hijo, tNodoArbolNario **pPadre)
{
    tParPadre parEncontrado;
    int resultado;

    resultado = BuscarPrimeroCondicionLista(MapaPadres, &parEncontrado, sizeof(parEncontrado), CondEsHijo, Hijo);

    if(resultado)
    {
        if(pPadre)
            *pPadre = parEncontrado.Padre;

        return 1;
    }
    return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Profundidad(tLista *MapaPadres, tNodoArbolNario *Nodo)
{
    int profundidad = 0;
    tNodoArbolNario* padreAux;

    while(BuscarPadre(MapaPadres, Nodo, &padreAux))
    {
        profundidad++;
        Nodo = padreAux;
    }

    return profundidad;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CondEsHijo(const void *Elem, const void *Contexto)
{
    const tParPadre* par = (const tParPadre *)Elem;
    return par->Hijo == (const tNodoArbolNario *)Contexto;
}
