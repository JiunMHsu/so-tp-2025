#include "cliente/kernel.h"
#include <stdlib.h>
#include <pthread.h>

void *atender_interrupt(void *);

int main(int argc, char *argv[])
{
    iniciar_config();

    int32_t fd_conexion_dispatch = conectar_kernel_dispatch();

    if (fd_conexion_dispatch == -1)
    {
        return EXIT_FAILURE;
    }

    int32_t fd_conexion_interrupt = conectar_kernel_interrupt();

    if (fd_conexion_interrupt == -1)
    {
        return EXIT_FAILURE;
    }

    // Implementar conexion con memoria

    // Hilo para la escucha de interrupciones
    pthread_t hilo_interrupt;

    pthread_create(&hilo_interrupt, NULL, &atender_kernel_interrupt, &fd_conexion_interrupt);
    pthread_detach(hilo_interrupt);

    // Escucha de dispatch
    while (1)
    {
        char *mensaje = recibir_mensaje(fd_conexion_dispatch);

        if (mensaje == NULL)
        {
            cerrar_conexion(fd_conexion_dispatch);
            return EXIT_FAILURE;
        }

        printf("Mensaje por dispatch: %s", mensaje);
    }

    return EXIT_SUCCESS;
}