#ifndef CPU_CONFIG_H
#define CPU_CONFIG_H

#include <errno.h>
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

void iniciar_config(void);
void destruir_config(void);
kernel_address get_kernel_address();
mem_address get_memoria_address();

#endif