#include "kernel.h"

static int32_t fd_dispatch;
static int32_t fd_interrupt;

static int32_t conectar_kernel_dispatch(void);
static int32_t conectar_kernel_interrupt(void);

int8_t conectar_kernel(char *id_cpu)
{
    fd_dispatch = conectar_kernel_dispatch();
    fd_interrupt = conectar_kernel_interrupt();

    if (fd_interrupt == -1 || fd_dispatch == -1)
        return -1;

    enviar_mensaje(id_cpu, fd_dispatch);
    return 0;
}

static int32_t conectar_kernel_dispatch(void)
{
    kernel_address datos_kernel = get_kernel_address();
    int32_t fd_conexion_dispatch = crear_conexion(datos_kernel.ip, datos_kernel.puerto_dispatch);
    int32_t respuesta_conexion_dispatch = handshake(fd_conexion_dispatch, CPU);

    if (respuesta_conexion_dispatch == -1)
    {
        log_mensaje_error("No se puedo establecer la conexion con el puerto de dispatch.");
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
        log_mensaje_error("No se puedo establecer la conexion con el puerto de interrupcion.");
        return -1;
    }

    return fd_conexion_interrupt;
}

void cerrar_conexion_kernel(void)
{
    log_mensaje_error("Cerrando conexión con kernel...");
    cerrar_conexion(fd_dispatch);
    cerrar_conexion(fd_interrupt);
}

t_peticion_ejecucion *recibir_peticion_ejecucion_kernel()
{
    return recibir_peticion_ejecucion(fd_dispatch);
}

int32_t recibir_interrupt()
{
    return recibir_senial(fd_interrupt);
}

void enviar_desalojo_kernel(u_int32_t pid, u_int32_t program_counter, motivo_desalojo motivo, char *syscall)
{
    t_desalojo *desalojo = crear_desalojo(pid, program_counter, motivo, syscall);
    enviar_desalojo(fd_dispatch, desalojo);
    destruir_desalojo(desalojo);
}
