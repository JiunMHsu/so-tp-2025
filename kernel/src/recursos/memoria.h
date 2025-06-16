#ifndef RECURSOS_MEMORIA_H
#define RECURSOS_MEMORIA_H

#include <stdlib.h>
#include <pthread.h>
#include <utils/sockets/sockets.h>
#include <utils/protocol/protocol.h>
#include <utils/mem_request/kernel.h>

#include "config/config.h"
#include "logger/logger.h"

/**
 * @brief Solicita la creación de un proceso en memoria.
 *
 * @param pid Identificador del proceso.
 * @param tamanio Tamaño del proceso.
 * @param ruta_codigo Ruta del archivo de código del proceso.
 * @return int8_t 1 si la solicitud fue exitosa, 0 en caso contrario.
 * @note En caso de algún error de conexión o respuesta, paniquea y finaliza el proceso.
 */
int8_t solicitar_creacion_proceso(u_int32_t pid, u_int32_t tamanio, char *ruta_codigo);

/**
 * @brief Solicita la finalización de un proceso en memoria.
 *
 * @param pid Identificador del proceso a finalizar.
 * @return int8_t 1 si la solicitud fue exitosa, 0 en caso contrario.
 * @note En caso de algún error de conexión o respuesta, paniquea y finaliza el proceso.
 */
int8_t solicitar_finalizacion_proceso(u_int32_t pid);

/**
 * @brief Solicita un volcado de memoria de un proceso.
 *
 * @param pid Identificador del proceso cuyo volcado se solicita.
 * @return int8_t 1 si la solicitud fue exitosa, 0 en caso contrario.
 * @note En caso de algún error de conexión o respuesta, paniquea y finaliza el proceso.
 */
int8_t solicitar_dump_proceso(u_int32_t pid);

/**
 * @brief Solicita un intercambio de memoria (swap out) de un proceso.
 *
 * @param pid Identificador del proceso a intercambiar.
 * @return int8_t 1 si la solicitud fue exitosa, 0 en caso contrario.
 * @note En caso de algún error de conexión o respuesta, paniquea y finaliza el proceso.
 */
int8_t solicitar_swap_out(u_int32_t pid);

/**
 * @brief Solicita un intercambio de memoria (swap in) de un proceso.
 *
 * @param pid Identificador del proceso a intercambiar.
 * @return int8_t 1 si la solicitud fue exitosa, 0 en caso contrario.
 * @note En caso de algún error de conexión o respuesta, paniquea y finaliza el proceso.
 */
int8_t solicitar_swap_in(u_int32_t pid);

#endif // RECURSOS_MEMORIA_H
