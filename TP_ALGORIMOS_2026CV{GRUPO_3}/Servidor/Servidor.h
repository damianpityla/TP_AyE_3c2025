#ifndef SERVIDOR_H_INCLUDED
#define SERVIDOR_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#include "../Primitivas/Cola.h"
#include "../Primitivas/Arbol_Binario.h"
#include "../Primitivas/Lista.h"

#include "../Juego_Volcan/Volcan.h"
#include "../Juego_Volcan/Juego.h"
#include "../Juego_Volcan/Usuario.h"

int iniciar_entorno_socket();
SOCKET conectar_servidor(const char *ip, int puerto);
int enviar_partida(SOCKET sock, int id_jugador, float puntos, int movimientos);
void ejecutar_servidor(int puerto, tArbolBin* pIndice);
void procesar_peticiones_pendientes(tCola* pCola, SOCKET client_sock, tArbolBin *pIndice);

int cmpIndicePorNombre(const void *a, const void *b);
int altaJugadorServidor(const char *arch, tArbolBin *pIndice, const char *nombre, tCmp cmp);
int registrarPartidaEnServidor(const char *arch, int id, float pts, int movs);
int CargarIndiceJugadores(const char *ArchJugadores, tArbolBin *pIndice);
int cmpRankingPorPuntos(const void *a, const void *b);
tNodoArbolBin** BuscarPorIdEnArbol(tArbolBin *p, int id);
void CargarIndiceBinario(tArbolBin *pIndice, const char *nombreArchivo);
void GuardarIndiceBinario(tArbolBin raiz, const char *nombreArchivo);


#endif // SERVIDOR_H_INCLUDED
