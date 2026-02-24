#include "Volcan.h"
#include "../Config/Configuracion.h"
int GenerarEstructuraVolcan(tEstado* estado, tConfig* config)
{
    tInfoCamara infoRaiz;
    int camarasCreadas = 1;

    infoRaiz.id = 0;
    infoRaiz.es_salida = 1;
    infoRaiz.hay_premio = 0;
    infoRaiz.hay_vida = 0;
    infoRaiz.hay_lava = 0;
    infoRaiz.cant_criaturas = 0;

    estado->Volcan = (tNodoArbolNario*)malloc(sizeof(tNodoArbolNario));
    if (!estado->Volcan) return 0;

    estado->Volcan->info = malloc(sizeof(tInfoCamara));
    if (!estado->Volcan->info) {
        free(estado->Volcan);
        return 0;
    }

    memcpy(estado->Volcan->info, &infoRaiz, sizeof(tInfoCamara));
    estado->Volcan->hijos = NULL;

    CrearRamasAleatorias(estado->Volcan, 0, config, &camarasCreadas);

    return 1;
}
tNodoArbolNario* CrearNodoCamara(int* proximoId)
{
    tNodoArbolNario* nuevoNodo;
    tInfoCamara* info;

    nuevoNodo = (tNodoArbolNario*)malloc(sizeof(tNodoArbolNario));
    if (!nuevoNodo) return NULL;

    info = (tInfoCamara*)malloc(sizeof(tInfoCamara));
    if (!info)
    {
        free(nuevoNodo);
        return NULL;
    }
    info->id = *proximoId;
    *proximoId = *proximoId + 1;

    info->hay_lava = 0;
    info->hay_premio = (rand() % 100 < 15);
    info->hay_vida = (rand() % 100 < 5);
    info->cant_criaturas = 0;
    info->es_salida = 0;

    nuevoNodo->info = info;
    nuevoNodo->hijos = NULL;

    return nuevoNodo;
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
        nuevaInfo.hay_premio = (rand() % 100 < 15);
        nuevaInfo.hay_vida = (rand() % 100 < 5);
        nuevaInfo.cant_criaturas = 0;

        if(AgregarHijo(padre, &nuevaInfo, sizeof(tInfoCamara), &nodoHijoCreado))
        {
            CrearRamasAleatorias(nodoHijoCreado, nivelActual + 1, config, proximoId);
        }
    }
}
void PoblarCamaras(tEstado* estado, struct sConfig* config)
{
    tLista listaAux;
    tNodoArbolNario* camara = NULL;
    tInfoCamara* datos = NULL;
    int i;

    CrearLista(&listaAux);

    if(estado->Volcan == NULL) return;

    CensoDeCamaras(estado->Volcan, &listaAux);

    if(listaAux == NULL) return;

    camara = SortearCamaraVacia(&listaAux);
    if (camara != NULL)
    {
        estado->PosJugador = camara;
    }
    else
    {
        VaciarLista(&listaAux);
        return;
    }

    for (i = 0; i < config->criaturas; i++)
    {
        camara = SortearCamaraCualquiera(&listaAux);
        if (camara && camara->info)
        {
            datos = (tInfoCamara*)camara->info;
            datos->cant_criaturas++;
        }
    }

    for (i = 0; i < config->vidas_extra; i++)
    {
        camara = SortearCamaraVacia(&listaAux);
        if (camara && camara->info)
        {
            ((tInfoCamara*)camara->info)->hay_vida = 1;
        }
    }

    for (i = 0; i < config->premios; i++)
    {
        camara = SortearCamaraVacia(&listaAux);
        if (camara && camara->info)
        {
            ((tInfoCamara*)camara->info)->hay_premio = 1;
        }
    }

    VaciarLista(&listaAux);
}

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
tNodoArbolNario* SortearCamaraCualquiera(tLista* listaPunteros)
{
    tLista it;
    int cant = 0;
    int pos;
    int i;
    tNodoArbolNario* res = NULL;

    if (!listaPunteros || !(*listaPunteros))
        return NULL;

    it = *listaPunteros;
    while(it)
    {
        cant++;
        it = it->sig;
    }

    pos = rand() % cant;
    it = *listaPunteros;

    for (i = 0; i < pos; i++)
        it = it->sig;

    res = *(tNodoArbolNario**)it->Info;

    return res;
}
tNodoArbolNario* SortearCamaraVacia(tLista* listaPunteros)
{
    tLista *it;
    tNodoLista* elim;
    int cant = 0, pos, i;
    tNodoArbolNario* res = NULL;

    it = listaPunteros;
    while (*it)
    {
        cant++;
        it = &((*it)->sig);
    }

    if(!cant)
        return NULL;

    pos = rand() % cant;
    it = listaPunteros;
    for (i = 0; i < pos; i++)
        it = &((*it)->sig);

    elim = *it;
    res = *(tNodoArbolNario**)elim->Info;

    *it = elim->sig;
    free(elim->Info);
    free(elim);

    return res;
}
void PosicionarJugadorEnInicio(tEstado *estado)
{
    tNodoArbolNario *hoja = ObtenerPrimeraHoja(estado->Volcan);
    if(!hoja)
    {
        estado->Volcan = hoja;
    }
}
void GrabarArchivoVolcan(tNodoArbolNario* raiz, const tEstado* estado, const char *NombreArchivo)
{
    FILE* pVolcan;

    pVolcan = fopen(NombreArchivo, "wt");
    if(!pVolcan)
        return;

    DibujarArbolEnArchivo(pVolcan, raiz, estado, 0, 75);

    fclose(pVolcan);
}

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
void ActualizarMapaPadres(tNodoArbolNario* raiz, tLista* mapaPadres)
{
    ConstruirMapaPadres(raiz, mapaPadres);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int ObtenerProfundidadCamara(tLista* mapaPadres, tNodoArbolNario* nodo)
{
    return Profundidad(mapaPadres, nodo);
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
///es como encontrar el nivel (segun copilot es menos laburo asi)
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
