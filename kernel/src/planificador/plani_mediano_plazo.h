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
#include "plani_corto_plazo.h"

void inicializar_planificador_mediano_plazo(q_estado *blocked,
                                            q_estado *q_susp_blocked,
                                            q_estado *q_susp_ready);

void insertar_en_blocked(t_pcb *proceso);

/**
 * @brief Desbloquea un proceso y lo reinsertar en la cola correspondiente.
 *
 * @param pid PID del proceso a desbloquear.
 * @param resultado Resultado de la operación que causó el desbloqueo, 0 si fue exitoso, -1 si falló.
 */
void desbloquear_proceso(u_int32_t pid, int8_t resultado);

#endif // PLANIFICADOR_MEDIANO_PLAZO_H
