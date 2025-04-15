#ifndef IO_CONFIG_H
#define IO_CONFIG_H

#include <stdlib.h>
#include <string.h>
#include <commons/config.h>
#include <commons/log.h>

typedef struct
{
    char *ip;
    char *puerto;
} kernel_address;

void iniciar_config(void);
void destruir_config(void);

kernel_address get_kernel_address(void);
t_log_level get_log_level(void);

#endif // IO_CONFIG_H
