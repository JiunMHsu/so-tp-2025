#ifndef CPU_CACHE_H
#define CPU_CACHE_H

#include <stdlib.h>
#include <string.h>

#include "config/config.h"
#include "logger/logger.h"
#include "ciclo_instruccion/ciclo_instruccion.h"

typedef struct
{
    u_int32_t pagina;
    void *contenido;
    u_int32_t bit_uso;
    u_int32_t bit_modificado;
} entrada_cache;

void inicializar_cache();
void cachear_pagina(u_int32_t nro_pagina, u_int32_t marco);
void escribir_cache(u_int32_t numero_pagina, u_int32_t offset, void *datos, u_int32_t buffer_size);
u_int32_t existe_pagina_cache(u_int32_t nro_pagina);
u_int32_t cache_habilitada();
void eliminar_entradas_cache();
void destruir_cache();

#endif // CPU_CACHE_H
