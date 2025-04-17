#include "logger.h"

t_log *cpu_logger;

void iniciar_logger()
{
    cpu_logger = log_create(LOG_FILE, "CPU", true, get_log_level());
}

void destruir_logger()
{
    log_destroy(cpu_logger);
}

void log_mensaje_error(char *error)
{
    log_error(cpu_logger, "%s", error);
}

void log_fetch_instruccion(u_int32_t pid, u_int32_t pc)
{
    log_trace(cpu_logger, "##PID : %d - FETCH - Program Counter : %d", pid, pc);
}

void log_instruccion_recibida()
{
    log_trace(cpu_logger, "## Llega interrupción al puerto Interrupt");
}

void log_instruccion_ejecutada(u_int32_t pid, char *instruccion, char *parametros) // parametros podria cambiar
{
    log_trace(cpu_logger, "## PID: %d - Ejecutando: %s - %s", pid, instruccion, parametros);
}

void log_operacion_acceso_memoria(u_int32_t pid, t_operacion_acceso accion, u_int32_t direccion_fisica, char *valor)
{
    char *accion_str = NULL;

    switch (accion)
    {
    case LECTURA:
        accion_str = "LEER";
        break;
    case ESCRITURA:
        accion_str = "ESCRITURA";
        break;
    }

    log_trace(cpu_logger, "PID : %d - Acción : %s - Dirección Física : %d - Valor : %s", pid, accion_str, direccion_fisica, valor);
}

void log_obtener_marco(u_int32_t pid, u_int32_t numero_pagina, u_int32_t numero_marco) // revisar parametros
{
    log_trace(cpu_logger, "PID: %d - OBTENER MARCO - Página: %d- Marco: %d", pid, numero_pagina, numero_pagina);
}

void log_tlb_hit(u_int32_t pid, u_int32_t numero_pagina) // revisar parametros
{
    log_trace(cpu_logger, "PID: %d - TLB HIT - Pagina: %d", pid, numero_pagina);
}

void log_tlb_miss(u_int32_t pid, u_int32_t numero_pagina)
{
    log_trace(cpu_logger, "PID: %d - TLB MISS - Pagina: %d", pid, numero_pagina);
}

void log_pagina_encontrada_cache(u_int32_t pid, u_int32_t numero_pagina)
{
    log_trace(cpu_logger, "PID: %d - Cache Miss - Pagina: %d", pid, numero_pagina);
}

void log_pagina_ingresada_cache(u_int32_t pid, u_int32_t numero_pagina)
{
    log_trace(cpu_logger, "PID: %d - Cache Add - Pagina: %d", pid, numero_pagina);
}

void log_pagina_actualizada_cache_memoria(u_int32_t pid, u_int32_t numero_pagina, u_int32_t frame_memoria_principal)
{
    log_trace(cpu_logger, "PID: %d - Memory Update - Página: %d - Frame: %d", pid, numero_pagina, frame_memoria_principal);
}
