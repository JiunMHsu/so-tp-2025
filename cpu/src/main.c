#include "cliente/kernel.h"
#include "cliente/memoria.h"
#include <stdlib.h>

int main(int argc, char *argv[])
{
    iniciar_config();
    iniciar_logger();

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
    int32_t fd_conexion_memoria = conectar_memoria();

    if (fd_conexion_memoria == -1)
    {
        return EXIT_FAILURE;
    }

    // Hilo para la escucha de interrupciones
    pthread_t hilo_interrupt;

    pthread_create(&hilo_interrupt, NULL, &atender_kernel_interrupt, &fd_conexion_interrupt);
    pthread_detach(hilo_interrupt);

    pthread_t hilo_dispatch;

    pthread_create(&hilo_dispatch, NULL, &atender_kernel_dispatch, &fd_conexion_dispatch);
    pthread_detach(hilo_dispatch);

    pthread_t hilo_memoria;
    pthread_create(&hilo_memoria, NULL, &atender_memoria, &fd_conexion_memoria);
    pthread_detach(hilo_memoria);

    pause();

    return EXIT_SUCCESS;
}