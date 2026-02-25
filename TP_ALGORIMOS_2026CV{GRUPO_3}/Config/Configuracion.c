#include "Configuracion.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CrearArchivoConfiguracion(const char *ArchConfiguracion)
{
    FILE *pConfig = fopen(ArchConfiguracion, "wt");
    if(!pConfig)
        return ERROR_ARCHIVO;

    fprintf(pConfig, "ALTURA_MAXIMA:4\n");
    fprintf(pConfig, "MAX_CONEXIONES:3\n");
    fprintf(pConfig, "VIDAS_JUGADOR:3\n");
    fprintf(pConfig, "VIDAS_EXTRA:2\n");
    fprintf(pConfig, "CRIATURAS:2\n");
    fprintf(pConfig, "PREMIOS:1\n");

    fclose(pConfig);
    printf("Se ha creado el archivo %s con valores iniciales.\n", ArchConfiguracion);

    return TODO_OK;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CargarConfiguracion(const char *ArchConfiguracion, tConfig *config)
{
    char linea[TAM_LINEA];
    char clave[TAM_CLAVE];
    int valor;

    FILE *pConfig = fopen(ArchConfiguracion, "rt");
    if(!pConfig)
    {
        CrearArchivoConfiguracion(ArchConfiguracion);
        printf("Archivo %s no encontrado. Se procedera a configurar los valores por defecto.", ArchConfiguracion);
        ConfigurarPorDefecto(config);
        return TODO_OK;
    }


    while(fgets(linea, sizeof(linea), pConfig))
    {
        if(sscanf(linea, "%[^=]=%d", clave, &valor) == 2 || sscanf(linea, "%[^:]:%d", clave, &valor) == 2)
        {
            if (strstr(clave, "ALTURA_MAXIMA"))
                config->altura_maxima = valor;

            else if (strstr(clave, "MAX_CONEXIONES"))
                config->max_conexiones = valor;

            else if (strstr(clave, "VIDAS_JUGADOR"))
                config->vidas_iniciales = valor;

            else if (strstr(clave, "VIDAS_EXTRA"))
                config->vidas_extra = valor;

            else if (strstr(clave, "CRIATURAS"))
                config->criaturas = valor;

            else if (strstr(clave, "PREMIOS"))
                config->premios = valor;
        }
    }

    fclose(pConfig);
    return TODO_OK;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int ValidarConfiguracion(const tConfig *config)
{
    if (config->altura_maxima < 1)
        return CONFIG_INVALIDO;

    if (config->max_conexiones < 1)
        return CONFIG_INVALIDO;

    if (config->vidas_iniciales < 1)
        return CONFIG_INVALIDO;

    if (config->criaturas < 0 || config->premios < 0)
        return CONFIG_INVALIDO;

    return TODO_OK;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MostrarConfiguracion(const tConfig *config)
{
    int i;
    printf("\n\t%c", 201);

    for(i = 0; i < 42; i++)
        printf("%c", 205);

    printf("%c\n", 187);

    printf("\t%c  %-38s  %c\n", 186, "CONFIGURACION DEL VOLCAN CARGADA", 186);

    printf("\t%c", 204);
    for(i = 0; i < 42; i++) printf("%c", 205);

    printf("%c\n", 185);

    printf("\t%c %-25s | %12d %c\n", 186, "Altura Maxima", config->altura_maxima, 186);
    printf("\t%c %-25s | %12d %c\n", 186, "Max. Conexiones Hijas", config->max_conexiones, 186);
    printf("\t%c %-25s | %12d %c\n", 186, "Vidas Iniciales", config->vidas_iniciales, 186);
    printf("\t%c %-25s | %12d %c\n", 186, "Vidas Extra", config->vidas_extra, 186);
    printf("\t%c %-25s | %12d %c\n", 186, "Criaturas", config->criaturas, 186);
    printf("\t%c %-25s | %12d %c\n", 186, "Premios", config->premios, 186);

    printf("\t%c", 200);
    for (int i = 0; i < 42; i++) printf("%c", 205);
    printf("%c\n\n", 188);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ConfigurarPorDefecto(tConfig *config)
{
    config->altura_maxima = ALTURA_MAX;
    config->max_conexiones = MAX_CONEXIONES;
    config->vidas_iniciales = VIDAS_INICIALES;
    config->vidas_extra = VIDAS_EXTRA;
    config->criaturas = CRIATURAS;
    config->premios = PREMIOS;
}
