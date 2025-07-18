#ifndef PLANIFICADOR_MEDIANO_PLAZO_H
#define PLANIFICADOR_MEDIANO_PLAZO_H

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#include "config/config.h"
#include "logger/logger.h"
#include "estado/estado.h"
#include "pcb/pcb.h"
#include "recursos/memoria.h"
#include "plani_corto_plazo.h"
#include "plani_largo_plazo.h"

typedef struct
{
    sem_t *hay_proceso;
    t_pcb *proceso;      // Proceso asociado al cronómetro
    u_int8_t esta_libre; // 1 si está libre, 0 si está ocupado
    u_int64_t tiempo;    // Tiempo de espera en milisegundos
    pthread_t rutina_consumo;
} t_cronometro;

void inicializar_planificador_mediano_plazo(void);

void insertar_en_blocked(t_pcb *proceso);

/**
 * @brief Desbloquea un proceso y lo reinsertar en la cola correspondiente.
 *
 * @param proceso pcb del proceso a desbloquear.
 * @param fallo Resultado de la operación que causó el desbloqueo, `0` si fue exitoso, `1` si falló.
 */
void desbloquear_proceso(t_pcb *proceso, u_int8_t fallo);

/**
 * @brief Intenta desuspender un proceso de la cola de suspended_ready.
 * Realiza una petición a la memoria para y se bloquea hasta que ésta responda.
 *
 * @return `t_pcb*`
 * @note Fcunción bloqueante si no hay procesos en suspender_ready.
 * @note Si la petición a memoria falla (no hay espacio suficiente para desuspender), retorna `NULL`.
 */
t_pcb *desuspender_proceso_ready(void);

/**
 * @brief Verifica si hay algún proceso en la cola de suspended_ready.
 *
 * @return `1` si hay al menos un proceso, `0` si no hay.
 */
int8_t hay_proceso_susp_ready(void);

#endif // PLANIFICADOR_MEDIANO_PLAZO_H
