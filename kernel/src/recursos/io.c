#include "io.h"

// colección global de IOs

void iniciar_io()
{
    // TODO: inicializar estructura de datos para manejar IOs
}

void *manejar_conexion_io(void *fd_ptr)
{
    int32_t fd_io = *((int32_t *)fd_ptr);
    free(fd_ptr);

    if (recibir_cliente(fd_io) != IO)
    {
        log_mensaje_error("Error cliente inválido");
        return NULL;
    }

    // Escuchar nombre de IO

    // TODO: Lógica de atención
    while (1)
    {
        enviar_mensaje("mensaje desde kernel", fd_io);
        sleep(2);
    }

    return NULL;
}
