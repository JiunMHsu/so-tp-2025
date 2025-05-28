#ifndef RECURSOS_MEMORIA_H
#define RECURSOS_MEMORIA_H

#include <stdlib.h>
#include <pthread.h>
#include <utils/sockets/sockets.h>
#include <utils/protocol/protocol.h>

#include "config/config.h"
#include "logger/logger.h"

/**
 * @brief Solicita la creación de un proceso en memoria.
 *
 * @param pid Identificador del proceso.
 * @param tamanio Tamaño del proceso.
 * @param ruta_codigo Ruta del archivo de código del proceso.
 * @return int32_t 1 si la solicitud fue exitosa, 0 en caso contrario. -1 si hubo un error de conexión.
 */
int32_t solicitar_creacion_proceso(u_int32_t pid, u_int32_t tamanio, char *ruta_codigo);

/**
 * @brief Solicita la finalización de un proceso en memoria.
 *
 * @param pid Identificador del proceso a finalizar.
 * @return int32_t 1 si la solicitud fue exitosa, 0 en caso contrario. -1 si hubo un error de conexión.
 */
int32_t solicitar_finalizacion_proceso(u_int32_t pid);
// probablemente solicitudes de swapping

#endif // RECURSOS_MEMORIA_H
