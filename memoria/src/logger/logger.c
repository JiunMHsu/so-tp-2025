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
