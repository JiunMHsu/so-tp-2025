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