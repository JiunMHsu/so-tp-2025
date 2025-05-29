#include "logger.h"

t_log *kernel_logger;

void iniciar_logger(t_log_level log_level)
{
    kernel_logger = log_create(LOG_FILE, "Kernel", true, log_level);
}

void destruir_logger(void)
{
    log_destroy(kernel_logger);
}

void log_evento(char *evento)
{
    log_info(kernel_logger, "%s", evento);
}

void log_mensaje_error(char *error)
{
    log_error(kernel_logger, "%s", error);
}

void log_syscall_recibida(u_int32_t pid, char *syscall)
{
    log_info(kernel_logger, "## (%d) - Solicitó syscall: %s", pid, syscall);
}

void log_creacion_proceso(u_int32_t pid)
{
    log_info(kernel_logger, "## (%d) Se crea el proceso - Estado: NEW", pid);
}

void log_cambio_de_estado(u_int32_t pid, t_state anterior, t_state actual)
{
    char *anterior_str = get_estado_string(anterior);
    char *actual_str = get_estado_string(actual);
    log_info(kernel_logger, "## (%d) Pasa del estado %s al estado %s", pid, anterior_str, actual_str);
}

void log_motivo_bloqueo(u_int32_t pid, char *dispositivo_io)
{
    log_info(kernel_logger, "## (%d) - Bloqueado por IO: %s", pid, dispositivo_io);
}

void log_fin_de_io(u_int32_t pid)
{
    log_info(kernel_logger, "## (%d) finalizó IO y pasa a READY", pid);
}

void log_desalojo_srt(u_int32_t pid)
{
    log_info(kernel_logger, "## (%d) - Desalojado por fin de SRT", pid);
}

void log_finalizacion_proceso(u_int32_t pid)
{
    log_info(kernel_logger, "## (%d) - Finaliza el proceso", pid);
}

// TODO: Implementar una vez que se hayan definido las métricas
void log_metricas_proceso(u_int32_t pid, t_list *metricas_estado, t_list *metricas_tiempo)
{
}
