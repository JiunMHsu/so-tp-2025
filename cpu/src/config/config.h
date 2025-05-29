#ifndef CPU_CONFIG_H
#define CPU_CONFIG_H

#include <errno.h>
#include <commons/config.h>
#include <commons/log.h>

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
t_log_level get_log_level();

#endif // CPU_CONFIG_H
