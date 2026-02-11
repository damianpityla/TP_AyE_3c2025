#ifndef SERVIDOR_H_INCLUDED
#define SERVIDOR_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#include "../Primitivas/Cola.h"
#include "../Primitivas/Arbol_Binario.h"
#include "Usuario.h"


int cmpIndicePorNombre(const void *a, const void *b);

#endif // SERVIDOR_H_INCLUDED
