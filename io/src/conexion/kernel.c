#include "kernel.h"

int32_t conectar_con_kernel(char *nombre_interfaz)
{
    kernel_address address = get_kernel_address();
    int32_t fd_kernel = crear_conexion(address.ip, address.puerto);
    if (handshake(fd_kernel, IO) == -1)
    {
        cerrar_conexion(fd_kernel);
        return -1;
    }

    enviar_mensaje(nombre_interfaz, fd_kernel);
    return fd_kernel;
}
