#ifndef USUARIO_H_INCLUDED
#define USUARIO_H_INCLUDED

#include "Juego.h"
#include "../Primitivas/Arbol_Binario.h"


#define TAM_NOMBRE 50

#define TODO_OK 1
#define ERROR_ARCHIVO -1

typedef struct
{
    int id;
    char nombre[TAM_NOMBRE + 1];
}tJugador;


typedef struct
{
    int idPartida;
    int idjugador;
    float Puntuacion;
    int CantMovimientos;
}tPartida;

typedef struct
{
    int id;
    char nombre[TAM_NOMBRE + 1];
    int desplazamiento;
}tIndiceJugador;

typedef struct
{
    int idJugador;
    char Nombre[TAM_NOMBRE + 1];
    float TotalPuntos;
    int partidas;
}tRanking;

typedef enum
{
    OPC_INICIAR_PARTIDA = 0,
    OPC_REGISTRARSE,
    OPC_VER_RANKING,
    OPC_DESCONECTAR
}eMenuOnline;

///////MENU
int         Menu                        ();
int         MenuOffLine                 ();
int         MenuOnLine                  ();
void        MostrarRanking              (const void *a);
int         SolicitarDatoUsuario        (char *bufferDato, const char *titulo);



#endif // USUARIO_H_INCLUDED
