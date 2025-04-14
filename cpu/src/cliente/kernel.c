#include "kernel.h"

int32_t conectar_kernel_dispatch(void)
{
    kernel_address datos_kernel = get_kernel_address();

    int32_t fd_conexion_dispatch = crear_conexion(datos_kernel.ip, datos_kernel.puerto_dispatch);

    int32_t respuesta_conexion_dispatch = handshake(fd_conexion_dispatch, CPU_DISPATCH);

    if (respuesta_conexion_dispatch == -1)
    {
        perror("No se puedo establecer la conexion con el servicio de dispatch.");
        cerrar_conexion(fd_conexion_dispatch);
        return -1;
    }

    return fd_conexion_dispatch;
}

int32_t conectar_kernel_interrupt(void)
{
    kernel_address datos_kernel = get_kernel_address();

    int32_t fd_conexion_interrupt = crear_conexion(datos_kernel.ip, datos_kernel.puerto_interrupt);

    int32_t respuesta_conexion_interrupt = handshake(fd_conexion_interrupt, CPU_INTERRUPT);

    if (respuesta_conexion_interrupt == -1)
    {
        perror("No se puedo establecer la conexion con el servicio de interrupcion.");
        cerrar_conexion(fd_conexion_interrupt);
        return -1;
    }

    return fd_conexion_interrupt;
}

void *atender_kernel_interrupt(void *fd_ptr)
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

    return NULL; // Revisar
}