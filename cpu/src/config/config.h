#ifndef CPU_CONFIG_H
#define CPU_CONFIG_H

#include <commons/config.h>

#define CONFIG_FILE "cpu.config"

typedef enum
{
    PUERTO_KERNEL_DISPATCH,
    PUERTO_KERNEL_INTERRUPT
} puerto_server;

typedef enum
{
    IP_KERNEL,
    IP_MEMORIA
} ip_server;

typedef struct
{
    char *ip;
    char *puerto_dispatch;
    char *puerto_interrupt;
} kernel_address;

typedef struct
{
    char *ip;
    char *puerto;
} mem_address;

char *iniciar_config();
char *destruir_config();
char *get_puerto_server(puerto_server puerto);
char *get_ip_server(ip_server ip);

#endif