#include "logger.h"

t_log *memoria_logger;

void iniciar_logger(t_log_level log_level)
{
    memoria_logger = log_create(LOG_FILE, "Memoria", true, log_level);
}

void destruir_logger(void)
{
    log_destroy(memoria_logger);
}

void log_evento(char *evento)
{
    log_info(memoria_logger, "%s", evento);
}

void log_mensaje_error(char *error)
{
    log_error(memoria_logger, "%s", error);
}

void log_mensaje_advertencia(char *advertencia)
{
    log_warning(memoria_logger, "%s", advertencia);
}

void log_conexion_kernel(int32_t socket_kernel)
{
    log_info(memoria_logger, "## Kernel Conectado - FD del socket: %d", socket_kernel);
}

void log_creacion_proceso(u_int32_t pid, u_int32_t tamanio)
{
    log_info(memoria_logger, "## PID: %d - Proceso Creado - Tamaño: %d", pid, tamanio);
}

void log_destruccion_proceso(u_int32_t pid, t_metricas *metricas)
{
    u_int32_t accesos_tablas = metricas->accesos_tablas;
    u_int32_t instrucciones_solicitadas = metricas->instrucciones_solicitadas;
    u_int32_t swap_outs = metricas->swap_outs;
    u_int32_t swap_ins = metricas->swap_ins;
    u_int32_t lecturas = metricas->lecturas;
    u_int32_t escrituras = metricas->escrituras;

    log_info(memoria_logger, "## PID: %d - Proceso Destruido - Métricas - Acc.T.Pag: %d; Inst.Sol.: %d; SWAP: %d; Mem.Prin.: %d; Lec.Mem.: %d; Esc.Mem.: %d",
             pid, accesos_tablas, instrucciones_solicitadas, swap_outs, swap_ins, lecturas, escrituras);
}

void log_obtencion_instruccion(u_int32_t pid, u_int32_t program_counter, char *instruccion)
{
    log_info(memoria_logger, "## PID: %d - Obtener instrucción: %d - Instrucción: %s", pid, program_counter, instruccion);
}

void log_acceso_espacio_usuario(u_int32_t pid, t_accion accion, u_int32_t direccion_fisica, u_int32_t tamanio)
{
    char *accion_str;

    switch (accion)
    {
    case ESCRITURA:
        accion_str = "Escritura";
        break;
    case LECTURA:
        accion_str = "Lectura";
        break;
    default:
        accion_str = "Operación desconocida";
        break;
    }

    log_info(memoria_logger, "## PID: %d - %s - Dir. Física: %d - Tamaño: %d", pid, accion_str, direccion_fisica, tamanio);
}

void log_memory_dump(u_int32_t pid)
{
    log_info(memoria_logger, "## PID: %d - Memory Dump solicitado", pid);
}

void log_swap_out_solicitado(u_int32_t pid)
{
    log_info(memoria_logger, "## PID: %d - Swap Out solicitado", pid);
}

void log_swap_in_solicitado(u_int32_t pid)
{
    log_info(memoria_logger, "## PID: %d - Swap In solicitado", pid);
}

void log_swap_out(u_int32_t pid)
{
    log_info(memoria_logger, "## PID: %d - Swap Out completado", pid);
}

void log_swap_in(u_int32_t pid)
{
    log_info(memoria_logger, "## PID: %d - Swap In completado", pid);
}
