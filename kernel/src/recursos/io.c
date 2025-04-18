#include "io.h"

void *manejar_conexion_io(void *fd_ptr)
{
    int32_t fd_io = *((int32_t *)fd_ptr);
    free(fd_ptr);

    if (recibir_cliente(fd_io) != IO)
    {
        log_mensaje_error("Error cliente inválido");
        return NULL;
    }

    // TODO: Lógica de atención
    while (1)
    {
        enviar_mensaje("mensaje desde kernel", fd_io);
        sleep(2);
    }

    return NULL;
}