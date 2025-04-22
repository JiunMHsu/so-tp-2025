#include "logger.h"

t_log *io_logger;

void iniciar_logger(t_log_level log_level)
{
    io_logger = log_create(LOG_FILE, "IO", true, log_level);
}

void destruir_logger()
{
    log_destroy(io_logger);
}

void log_mensaje_error(char *error)
{
    log_error(io_logger, "%s", error);
}

void log_inicio_io(u_int32_t pid, u_int32_t tiempo)
{
    log_info(io_logger, "## PID: %d - Inicio de IO - Tiempo: %d", pid, tiempo);
}

void log_finalizacion_io(u_int32_t pid)
{
    log_info(io_logger, "## PID: %d - Fin de IO", pid);
}

void log_error_peticion(char *mensaje)
{
    log_error(io_logger, "El siguiente mensaje no se recibio correctamente: %s", mensaje);
}
