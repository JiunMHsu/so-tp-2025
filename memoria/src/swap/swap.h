#ifndef SWAP_H

#include <stdlib.h>
#include <stdio.h>
#include <commons/collections/dictionary.h>

#include "config/config.h"
#include "logger/logger.h"

typedef struct
{
    u_int32_t offset;
    u_int32_t paginas_en_swap;
} t_swap_info;

void inicializar_swap();
void cerrar_swap();
void guardar_en_swap(u_int32_t pid, u_int32_t cantidad_paginas, void *origen);
void recuperar_proceso_de_swap(u_int32_t pid, void *destino);
void liberar_swap(u_int32_t pid);

#endif // SWAP_H
