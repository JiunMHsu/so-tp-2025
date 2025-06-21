#ifndef CPU_TLB_H
#define CPU_TLB_H

#include <stdlib.h>
#include <string.h>
#include "config/config.h"

typedef struct
{
    u_int32_t pagina;
    u_int32_t marco;
} entrada_tlb;

u_int32_t inicializar_tlb();
u_int32_t get_marco_tlb(u_int32_t pid, u_int32_t pagina);
void agregar_entrada_tlb(u_int32_t pagina, u_int32_t marco);
void destruir_tlb();

#endif //CPU_TLB_H