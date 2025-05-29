#ifndef KERNEL_LOGGER_H
#define KERNEL_LOGGER_H

#include <stdlib.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include <commons/log.h>
#include <pcb/pcb.h>

#define LOG_FILE "kernel.log"

void iniciar_logger(t_log_level log_level);
void destruir_logger(void);

void log_evento(char *evento);
void log_mensaje_error(char *error);

void log_syscall_recibida(u_int32_t pid, char *syscall);
void log_creacion_proceso(u_int32_t pid);
void log_cambio_de_estado(u_int32_t pid, t_state anterior, t_state actual);
void log_motivo_bloqueo(u_int32_t pid, char *dispositivo_io);
void log_fin_de_io(u_int32_t pid);
void log_desalojo_srt(u_int32_t pid);
void log_finalizacion_proceso(u_int32_t pid);
void log_metricas_proceso(u_int32_t pid, t_dictionary *metricas_estado, t_dictionary *metricas_tiempo);

#endif // KERNEL_LOGGER_H
