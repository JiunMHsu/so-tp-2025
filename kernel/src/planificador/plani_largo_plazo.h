#ifndef PLANIFICADOR_LARGO_PLAZO_H
#define PLANIFICADOR_LARGO_PLAZO_H

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#include "config/config.h"
#include "logger/logger.h"
#include "recursos/memoria.h"
#include "estado/estado.h"
#include "pcb/pcb.h"

void inicializar_planificador_largo_plazo(algoritmo_planificacion alg_planificacion,
                                          q_estado *q_new,
                                          q_estado *q_ready,
                                          q_estado *q_exit);

void insertar_proceso_nuevo(char *pseudocodigo, u_int32_t tamanio_proceso);

#endif // PLANIFICADOR_LARGO_PLAZO_H
