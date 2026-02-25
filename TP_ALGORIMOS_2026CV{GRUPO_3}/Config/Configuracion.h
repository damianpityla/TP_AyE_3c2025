#ifndef CONFIGURACION_H_INCLUDED
#define CONFIGURACION_H_INCLUDED

#include <stdio.h>
#include <string.h>

#define TODO_OK 1
#define ERROR_ARCHIVO -1
#define CONFIG_INVALIDO -2

#define TAM_LINEA 100
#define TAM_CLAVE 50

#define ALTURA_MAX         4
#define MAX_CONEXIONES     3
#define VIDAS_INICIALES    3
#define VIDAS_EXTRA        2
#define CRIATURAS          2
#define PREMIOS            1

#define ARCH_CONFIG "config.txt"

typedef struct sConfig
{
    int altura_maxima;
    int max_conexiones;
    int vidas_iniciales;
    int vidas_extra;
    int criaturas;
    int premios;
}tConfig;

int         CrearArchivoConfiguracion   (const char *ArchConfiguracion);
int         CargarConfiguracion         (const char *ArchConfiguracion, tConfig *config);
int         ValidarConfiguracion        (const tConfig *config);
void        MostrarConfiguracion        (const tConfig *config);
void        ConfigurarPorDefecto        (tConfig *config);

#endif // CONFIGURACION_H_INCLUDED
