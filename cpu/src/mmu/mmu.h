#ifndef CPU_MMU_H
#define CPU_MMU_H

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <commons/string.h>
#include <conexion/memoria.h>
#include "utils/mem_request/cpu.h"
#include "logger/logger.h"
#include "config/config.h"
#include "tlb/tlb.h"
#include "cache/cache.h"
#include "ciclo_instruccion/ciclo_instruccion.h"

void inicializar_mmu();
void destruir_mmu();
u_int32_t get_direccion_fisica(u_int32_t direccion_logica);
u_int32_t get_offset(u_int32_t direccion_logica);
u_int32_t get_nro_pagina(u_int32_t direccion_logica);
u_int32_t get_marco(u_int32_t direccion_logica);

#endif