#ifndef CPU_CONFIG_H
#define CPU_CONFIG_H

#include <commons/config.h>

#define CONFIG_FILE "cpu.config"

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
kernel_address get_kernel_address();

#endif