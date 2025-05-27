#ifndef KERNEL_ESTADO_H
#define KERNEL_ESTADO_H

#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <utils/mqueue/mqueue.h>
#include <utils/mlist/mlist.h>

#include "pcb/pcb.h"

typedef struct
{
    t_mutex_list *lista;
    sem_t *hay_proceso;
} q_estado;

/**
 * @brief Crea una cola de procesos para el estado dado.
 *
 * @return `q_estado*`
 */
q_estado *crear_estado();

/**
 * @brief Encola un proceso en el estado dado.
 *
 * @param estado
 * @param pcb
 */
void push_proceso(q_estado *estado, t_pcb *pcb);

/**
 * @brief Inserta un proceso en el estado dado de forma ordenada.
 *
 * @param estado
 * @param pcb
 */
void ordered_insert_proceso(q_estado *estado, t_pcb *pcb, int (*criterio)(t_pcb *, t_pcb *));

/**
 * @brief Desencola un proceso del estado dado.
 *        El puntero retornado nunca será NULL.
 *
 * @param estado
 * @return `t_pcb*`
 * @note Función bloqueante.
 */
t_pcb *pop_proceso(q_estado *estado);

/**
 * @brief Devuelve el primer proceso encolado en el estado dado.
 *        El puntero retornado nunca será NULL.
 *
 * @param estado
 * @return `t_pcb*`
 */
t_pcb *peek_proceso(q_estado *estado);

/**
 * @brief
 *
 * @param estado
 * @param pid
 * @return `t_pcb*`
 *
 * @note Si el proceso no existe, retorna NULL.
 */
t_pcb *remove_proceso(q_estado *estado, u_int32_t pid);

/**
 * @brief Dvueleve una lista de PIDs de los procesos encolados en el estado dado.
 *
 * @param estado
 * @return `t_list*`
 *
 * @note Los elementos de la lista son `u_int32_t*`.
 * @note Liberar la lista con `list_destroy_and_destroy_elements(lista, &free)`.
 */
t_list *get_pids(q_estado *estado);

/**
 * @brief Indica si hay algún proceso encolado en el estado dado.
 *
 * @param estado
 * @return `int8_t`
 */
int8_t hay_proceso(q_estado *estado);

/**
 * @brief Destruye un estado.
 *        Limpia la cola de procesos y libera la memoria,
 *        no libera los procesos (*pcb) contenidos.
 *
 * @param estado
 */
void destruir_estado(q_estado *estado);

#endif // KERNEL_ESTADO_H
