#ifndef CPU_LOGGER_H
#define CPU_LOGGER_H

#include <commons/log.h>
#include <commons/config.h>

#define LOG_FILE "cpu.log"

typedef enum
{
    ESCRITURA,
    LECTURA,
} t_operacion_acceso;

void iniciar_logger();
void destruir_logger();

void log_error();

void log_fetch_instruccion();
void log_interrupcion_recibida(u_int32_t pid, u_int32_t pc);
void log_instruccion_ejecutada(u_int32_t pid); // faltan params que no se definir
void log_operacion_acceso_memoria();
void log_obtener_marco();
void log_TLB_hit();
void log_TLB_miss();
void log_pagina_encontrada_cache();
void log_pagina_ingresada_cache();
void log_pagina_actualizada_cache_memoria();

#endif