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

int32_t conectar_memoria(void)
{
    mem_address datos_memoria = get_memoria_address();

    int32_t fd_conexion_memoria = crear_conexion(datos_memoria.ip, datos_memoria.puerto);

    int32_t respuesta_conexion_memoria = handshake(fd_conexion_memoria, CPU);

    if (respuesta_conexion_memoria == -1)
    {
        perror("No se puedo establecer la conexion con la memoria");
        cerrar_conexion(fd_conexion_memoria);
        return -1;
    }

    return fd_conexion_memoria;
}

void *atender_kernel_interrupt(void *fd_ptr)
{
    int32_t fd_conexion_interrupt = *((int32_t *)fd_ptr);

    while (1)
    {
        char *mensaje = recibir_mensaje(fd_conexion_interrupt);

        if (mensaje == NULL)
        {
            cerrar_conexion(fd_conexion_interrupt);
            return NULL;
        }

        printf("Mensaje enviado por interrupt: %s \n", mensaje);
        free(mensaje);
    }

    return NULL;
}

void *atender_kernel_dispatch(void *fd_ptr)
{
    int32_t fd_conexion_dispatch = *((int32_t *)fd_ptr);

    // Escucha de dispatch
    while (1)
    {
        char *mensaje = recibir_mensaje(fd_conexion_dispatch);

        if (mensaje == NULL)
        {
            cerrar_conexion(fd_conexion_dispatch);
            return NULL;
        }

        printf("Mensaje por dispatch: %s \n", mensaje);
        free(mensaje);
    }

    return NULL;
}

void *atender_memoria(void *fd_prt)
{
    int32_t fd_conexion_memoria = *((int *)fd_prt);

    while (1)
    {
        enviar_mensaje("Mensaje desde CPU", fd_conexion_memoria);
        sleep(2);
    }

    return NULL;
}