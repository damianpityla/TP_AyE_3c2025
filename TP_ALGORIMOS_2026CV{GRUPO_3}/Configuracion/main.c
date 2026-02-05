#include "Configuracion.h"

int main()
{
    tConfig config;
    int estado;

    estado = CargarConfiguracion(ARCH_CONFIG, &config);

    if(estado == TODO_OK)
    {
        MostrarConfiguracion(&config);
        if(ValidarConfiguracion(&config) == TODO_OK)
            printf("Configuracion validada correctamente.\n");
        else
            printf("Error: La configuracion no cumple con los limites del sistema.\n");
    }
    else
        printf("No se pudo establecer la configuracion del juego.\n");

    printf("\nPresione cualquier tecla para salir...");
    getchar();
    return 0;
}
