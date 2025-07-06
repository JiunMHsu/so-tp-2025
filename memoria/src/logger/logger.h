#ifndef MEMORIA_LOGGER_H
#define MEMORIA_LOGGER_H

#include <stdlib.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include <commons/log.h>

#include "metricas/metricas.h"

#define LOG_FILE "memoria.log"

typedef enum
{
    ESCRITURA,
    LECTURA,
} t_operacion_acceso;

typedef t_operacion_acceso t_accion;

void iniciar_logger(t_log_level log_level);
void destruir_logger(void);

void log_evento(char *evento);
void log_mensaje_error(char *error);
void log_mensaje_advertencia(char *advertencia);

void log_conexion_kernel(int32_t socket_kernel);
void log_creacion_proceso(u_int32_t pid, u_int32_t tamanio);
void log_destruccion_proceso(u_int32_t pid, t_metricas *metricas);
void log_obtencion_instruccion(u_int32_t pid, u_int32_t program_counter, char *instruccion);
void log_acceso_espacio_usuario(u_int32_t pid, t_accion accion, u_int32_t direccion_fisica, u_int32_t tamanio);
void log_memory_dump(u_int32_t pid);

#endif // MEMORIA_LOGGER_H
