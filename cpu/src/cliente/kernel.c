#include "kernel.h"

//conectar_kernel_dispatch
//conectar_kernel_interrupt
//retornan fd de conexion
//TODO
int32_t iniciar_conexion_kernel(void)
{
    // Inicia la conexion
    char *puerto_kernel_dispatch = get_puerto_server(PUERTO_KERNEL_DISPATCH);
    char *puerto_kernel_interrupt = get_puerto_server(PUERTO_KERNEL_INTERRUPT);

    char *ip_kernel = get_ip_server(IP_KERNEL);

    uint32_t fd_conexion_dispatch = crear_conexion(ip_kernel, puerto_kernel_dispatch;
    uint32_t fd_conexion_interrupt = crear_conexion(ip_kernel, puerto_kernel_interrupt);

    //Concreta la conexion
    uint32_t respuesta_conexion_dispatch = handshake(fd_conexion_dispatch, CPU_DISPATCH);

    if(respuesta_conexion_dispatch == -1)
    {
        perror("No se puedo establecer la conexion con el dispatch");
        cerrar_conexion(fd_conexion_dispatch);
        return -1;
    }

    uint32_t respuesta_interrupt = handshake(fd_conexion_interrupt, CPU_INTERRUPT);

    if(respuesta_interrupt == -1)
    {
        perror("No se puedo establecer la conexion con el interrupt");
        cerrar_conexion(fd_conexion_interrupt);
        return -1;
    }

    return ;
}

void finalizar_cliente()
{
}