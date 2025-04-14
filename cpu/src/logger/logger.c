#include "logger.h"

t_log *cpu_logger;

void iniciar_logger(t_log_level log_level)
{
    cpu_logger = log_create(LOG_FILE, "CPU", true, log_level);
}

void destruir_logger()
{
    log_destroy(cpu_logger);
}