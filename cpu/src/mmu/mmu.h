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

void inicializar_mmu();
u_int32_t get_direccion_fisica(u_int32_t pid, u_int32_t direccion_logica);
void destruir_mmu();

#endif