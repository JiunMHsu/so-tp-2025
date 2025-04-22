#include <stdlib.h>

#include "conexion/kernel.h"
#include "conexion/memoria.h"
#include "interrupcion/interrupcion.h"

int main(int argc, char *argv[])
{
    iniciar_config();
    iniciar_logger();

    t_kernel_sockets kernel_sockets = conectar_kernel();
    if (kernel_sockets.fd_dispatch == -1 || kernel_sockets.fd_interrupt == -1)
    {
        log_mensaje_error("No se pudo establecer la conexion con el kernel.");
        return EXIT_FAILURE;
    }

    if (conectar_memoria() == -1)
    {
        cerrar_conexion(kernel_sockets.fd_dispatch);
        cerrar_conexion(kernel_sockets.fd_interrupt);
        return EXIT_FAILURE;
    }

    inicializar_interrupcion(kernel_sockets.fd_interrupt);

    pause();

    return EXIT_SUCCESS;
}
