#ifndef CPU_CACHE_H
#define CPU_CACHE_H

#include <stdlib.h>
#include <string.h>
#include "config/config.h"

typedef struct
{
    u_int32_t pagina;
    void *contenido;
    u_int32_t bit_uso;
    u_int32_t bit_modificado;
} entrada_cache;

u_int32_t inicializar_cache();
// void agregar_entrada_cache();
u_int32_t pagina_en_cache();
u_int32_t cache_habilitada();
void escribir_pagina();
void leer_pagina();

#endif