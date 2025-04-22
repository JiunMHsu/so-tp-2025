#include "kernel.h"

static int32_t conectar_kernel_dispatch(void);
static int32_t conectar_kernel_interrupt(void);

t_kernel_sockets conectar_kernel(void)
{
    int32_t fd_conexion_dispatch = conectar_kernel_dispatch();
    int32_t fd_conexion_interrupt = conectar_kernel_interrupt();

    return (t_kernel_sockets){fd_conexion_dispatch, fd_conexion_interrupt};
}

static int32_t conectar_kernel_dispatch(void)
{
    kernel_address datos_kernel = get_kernel_address();
    int32_t fd_conexion_dispatch = crear_conexion(datos_kernel.ip, datos_kernel.puerto_dispatch);
    int32_t respuesta_conexion_dispatch = handshake(fd_conexion_dispatch, CPU);

    if (respuesta_conexion_dispatch == -1)
    {
        log_mensaje_error("No se puedo establecer la conexion con el servicio de dispatch.");
        cerrar_conexion(fd_conexion_dispatch);
        return -1;
    }

    return fd_conexion_dispatch;
}

static int32_t conectar_kernel_interrupt(void)
{
    kernel_address datos_kernel = get_kernel_address();
    int32_t fd_conexion_interrupt = crear_conexion(datos_kernel.ip, datos_kernel.puerto_interrupt);
    int32_t respuesta_conexion_interrupt = handshake(fd_conexion_interrupt, CPU);

    if (respuesta_conexion_interrupt == -1)
    {
        log_mensaje_error("No se puedo establecer la conexion con el servicio de interrupcion.");
        cerrar_conexion(fd_conexion_interrupt);
        return -1;
    }

    return fd_conexion_interrupt;
}
