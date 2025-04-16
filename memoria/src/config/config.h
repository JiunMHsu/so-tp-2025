#ifndef MEMORIA_CONFIG_H
#define MEMORIA_CONFIG_H

#include <errno.h>
#include <commons/config.h>
#include <commons/log.h>

#define CONFIG_FILE "memoria.config"

void iniciar_config(void);
void destruir_config(void);

char *get_puerto_escucha(void);
t_log_level get_log_level(void);
int32_t get_tam_memoria(void);
int32_t get_tam_pagina(void);
int32_t get_entradas_por_tabla(void);
int32_t get_cantidad_niveles(void);
int32_t get_retardo_memoria(void);
char *get_path_swapfile(void);
int32_t get_retardo_swap(void);
char *get_dump_path(void);

#endif // MEMORIA_CONFIG_H