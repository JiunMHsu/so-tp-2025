#ifndef PLANIFICADOR_CORTO_PLAZO_H
#define PLANIFICADOR_CORTO_PLAZO_H

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#include "config/config.h"
#include "logger/logger.h"
#include "pcb/pcb.h"
#include "estado/estado.h"

void inicializar_planificador_corto_plazo(algoritmo_planificacion alg_planificacion,
                                          q_estado *estado_ready);

#endif // PLANIFICADOR_CORTO_PLAZO_H
