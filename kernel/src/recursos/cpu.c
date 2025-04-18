#include "cpu.h"

// colección global de CPUs

void *manejar_conexion_dispatch(void *fd_cpu)
{
    int32_t fd_dispatch = *((int32_t *)fd_cpu);
    free(fd_cpu);

    if (recibir_cliente(fd_dispatch) != CPU)
    {
        log_mensaje_error("Error cliente inválido");
        return NULL;
    }

    // Escuchar identificador

    // TODO: Lógica de atención
    while (1)
    {
        enviar_mensaje("mensaje desde kernel", fd_dispatch);
        sleep(2);
    }

    return NULL;
}

void *manejar_conexion_interrupt(void *fd_cpu)
{
    int32_t fd_interrupt = *((int32_t *)fd_cpu);
    free(fd_cpu);

    if (recibir_cliente(fd_interrupt) != CPU)
    {
        log_mensaje_error("Error cliente inválido");
        return NULL;
    }

    // Escuchar identificador

    // TODO: Lógica de atención
    while (1)
    {
        enviar_mensaje("mensaje desde kernel", fd_interrupt);
        sleep(2);
    }

    return NULL;
}
