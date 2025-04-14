#include "cliente/kernel.h"
#include <stdlib.h>
#include <pthread.h>

void *atender_interrupt(void *);

int main(int argc, char *argv[])
{
    iniciar_config();

    int32_t resultado_conexion_kernel = iniciar_conexion_kernel();

    if (resultado_conexion_kernel == -1)
    {
        return EXIT_FAILURE;
    }

    pthread_t hilo_interrupt;

    pthread_create(&hilo_interrupt, NULL, &atender_interrupt, NULL); // en realidad recibe fd_conexio_interrupt
    pthread_detach(hilo_interrupt);

    while (1)
    {
        char *mensaje = recibir_mensaje(); // fd_conexio_dispatch

        printf("Mensaje por dispatch: " + mensaje);
    }

    return EXIT_SUCCESS;
}

void *atender_interrupt(void *fd_ptr) // cambiar param
{
    int32_t fd_interrupt = *((int32_t *)fd_ptr);

    while (1)
    {
        char *mensaje = recibir_mensaje(fd_interrupt);

        if (mensaje == NULL)
        {
            cerrar_conexion(fd_interrupt);
            return NULL;
        }

        printf("Mensaje enviado por interrupt: " + mensaje);
    }
}