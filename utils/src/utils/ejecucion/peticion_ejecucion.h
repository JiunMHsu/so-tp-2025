#ifndef UTILS_PETICION_EJECUCION_H
#define UTILS_PETICION_EJECUCION_H

#include <stdlib.h>
#include <stdint.h>
#include <utils/sockets/sockets.h>
#include <utils/protocol/protocol.h>

typedef struct
{
    uint32_t pid;
    uint32_t program_counter;
} t_peticion_ejecucion;

/**
 * @brief Crea una petición de ejecución.
 *
 * @param pid PID del proceso a ejecutar.
 * @param program_counter PC del proceso a ejecutar.
 *
 * @return `t_peticion_ejecucion*`
 */
t_peticion_ejecucion *crear_peticion_ejecucion(uint32_t pid, uint32_t program_counter);

/**
 * @brief Destruye una petición de ejecución.
 *
 * @param peticion puntero a la petición a destruir.
 */
void destruir_peticion_ejecucion(t_peticion_ejecucion *peticion);

/**
 * @brief Envía una petición de ejecución por dispatch.
 *
 * @param fd_dispatch
 * @param peticion
 */
void enviar_peticion_ejecucion(int32_t fd_dispatch, t_peticion_ejecucion *peticion);

/**
 * @brief Recibe una petición de ejecución por dispatch.
 *
 * @param fd_dispatch
 * @return `t_respuesta_ejecucion*`
 */
t_peticion_ejecucion *recibir_peticion_ejecucion(int32_t fd_dispatch);

#endif // UTILS_PETICION_EJECUCION_H
