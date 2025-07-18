#ifndef CPU_CONFIG_H
#define CPU_CONFIG_H

#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <commons/string.h>
#include <commons/config.h>
#include <commons/log.h>

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

typedef enum
{
    FIFO,
    LRU,
    CLOCK,
    CLOCK_M
} algoritmo_sustitucion;

void iniciar_config(char *id);
void destruir_config(void);
u_int32_t get_entradas_tlb(void);
u_int32_t get_entradas_cache(void);
algoritmo_sustitucion get_algoritmo_sustitucion_tlb();
algoritmo_sustitucion get_algoritmo_sustitucion_cache();
u_int32_t get_retardo();
kernel_address get_kernel_address();
mem_address get_memoria_address();
t_log_level get_log_level();

#endif // CPU_CONFIG_H
