#ifndef IO_LOGGER_H
#define IO_LOGGER_H

#include <stdlib.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include <commons/log.h>

#define LOG_FILE "io.log"

void iniciar_logger(t_log_level log_level);
void destruir_logger(void);

void log_inicio_io(u_int32_t pid);
void log_finalizacion_io(u_int32_t pid);

#endif // IO_LOGGER_H
