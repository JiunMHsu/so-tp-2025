#ifndef CPU_LOGGER_H
#define CPU_LOGGER_H

#include <commons/log.h>
#include <commons/config.h>
#include <utils/string/string.h>

#include "config/config.h"

typedef enum
{
    ESCRITURA,
    LECTURA,
} t_operacion_acceso;

void iniciar_logger(char *id_cpu);
void destruir_logger();

void log_mensaje_error(char *error);

void log_fetch_instruccion(u_int32_t pid, u_int32_t pc);
void log_interrupcion_recibida();
void log_instruccion_ejecutada(u_int32_t pid, char *instruccion, char *parametros); // parametros podria cambiar
void log_operacion_acceso_memoria(u_int32_t pid, t_operacion_acceso accion, u_int32_t direccion_fisica, char *valor);
void log_obtener_marco(u_int32_t pid, u_int32_t numero_pagina, u_int32_t numero_marco);
void log_tlb_hit(u_int32_t pid, u_int32_t numero_pagina);
void log_tlb_miss(u_int32_t pid, u_int32_t numero_pagina);
void log_pagina_ingresada_tlb(u_int32_t pid, u_int32_t numero_pagina);
void log_cache_hit(u_int32_t pid, u_int32_t numero_pagina);
void log_cache_miss(u_int32_t pid, u_int32_t numero_pagina);
void log_pagina_ingresada_cache(u_int32_t pid, u_int32_t numero_pagina);
void log_pagina_actualizada_cache_memoria(u_int32_t pid, u_int32_t numero_pagina, u_int32_t frame_memoria_principal);

#endif // CPU_LOGGER_H
