#include "cliente.h"

void iniciar_cliente()
{
    iniciar_config();

    char *puerto_kernel_dispatch = get_puerto_server(PUERTO_KERNEL_DISPATCH);
    char *puerto_kernel_interrupt = get_puerto_server(PUERTO_KERNEL_INTERRUPT);

    char *ip_kernel = get_ip_server(IP_KERNEL);

    int socket_cpu_dispatch = crear_conexion(ip_kernel, puerto_kernel_dispatch;
    int socket_cpu_interrupt = crear_conexion(ip_kernel, puerto_kernel_interrupt);
}

void finalizar_cliente()
{
}