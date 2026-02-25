#ifndef SERVIDOR_H_INCLUDED
#define SERVIDOR_H_INCLUDED

///              LIBRERIAS INCLUIDAS             ///
/*---          LIBRERIAS INSTALADAS      ---*/
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

/*---        LIBRERIAS PROPIAS          --- */
#include "../Primitivas/Cola.h"
#include "../Primitivas/Arbol_Binario.h"
#include "../Primitivas/Lista.h"
#include "../Juego_Volcan/Usuario.h"

///              MANEJOS DE ARCHIVOS             ///
#define ARCH_JUGADORES "Jugadores.dat"
#define ARCH_INDICE    "Jugadores.idx"
#define ARCH_PARTIDAS  "Partidas.dat"

///              CONSTANTES Y MACROS             ///
/* ---    MACROS DE CONFIGURACION DE RED  --- */
#define IP_SERVIDOR           "127.0.0.1"
#define SRV_DEFAULT_PORT      8080
#define PUERTO_DB             SRV_DEFAULT_PORT
#define SRV_BACKLOG           8
#define MAX_CONEXIONES_ESPERA 3

/* ---          MACROS DE TAMAÑOS         --- */
#define TAM_BUFFER            1024
#define TAM_PETICION          TAM_BUFFER
#define TAM_RESPUESTA         TAM_BUFFER
#define TAM_NOMBRE            50

/* ---      MACROS DE ESTADO DE ENVIO      --- */
#define ENVIO_OK              1
#define ENVIO_ERROR           0

/* ---           MACROS DE CMD             --- */
#define CMD_LOGIN             "LOGIN"
#define CMD_REGISTER          "REGISTER"
#define CMD_PARTIDA           "PARTIDA"
#define CMD_RANKING           "RANKING"
#define MSJ_OK_PARTIDA        "PARTIDA_GUARDADA"

/* ---         MACROS DE RESPUESTAS         --- */
#define MSJ_OK_PARTIDA        "PARTIDA_GUARDADA"
#define MSJ_REGISTER_OK       "REGISTER_OK"
#define MSJ_ERR_USER_NOT_FOUND "ERROR_USER_NOT_FOUND"
#define MSJ_ERR_ALREADY_EXISTS "ERROR_ALREADY_EXISTS"
#define MSJ_ERR_DB            "ERROR_DB"

int             iniciar_entorno_socket          ();
SOCKET          conectar_servidor               (const char *ip, int puerto);
int             enviar_partida                  (SOCKET sock, int id_jugador, float puntos, int movimientos);
void            ejecutar_servidor               (int puerto, tArbolBin* pIndice);
void            procesar_peticiones_pendientes  (tCola* pCola, SOCKET client_sock, tArbolBin *pIndice);
int             altaJugadorServidor             (const char *arch, tArbolBin *pIndice, const char *nombre, tCmp cmp);
int             registrarPartidaEnServidor      (const char *arch, int id, float pts, int movs);
int             cmpRankingPorPuntos             (const void *a, const void *b);
tNodoArbolBin** BuscarPorIdEnArbol              (tArbolBin *p, int id);
void            CargarIndiceBinario             (tArbolBin *pIndice, const char *nombreArchivo);
void            GuardarIndiceBinario            (tArbolBin raiz, const char *nombreArchivo);
int             GenerarRanking                  (const char *archPartidas, tArbolBin *pIndice, tLista *pRanking);
int             CargarIndiceJugadores           (const char *ArchJugadores, tArbolBin *pIndice);
void            AcumularRanking                 (void *nodoInfo, const void *elem);
int             cmpRankingPorId                 (const void *a, const void *b);
int             cmpIndicePorId                  (const void* a, const void* b);
int             cmpRankingPorPuntosDesc         (const void *a, const void *b);
int             cmpIndicePorNombre              (const void *a, const void *b);
#endif // SERVIDOR_H_INCLUDED
