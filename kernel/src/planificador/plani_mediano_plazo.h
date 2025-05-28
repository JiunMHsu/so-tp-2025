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

void inicializar_planificador_mediano_plazo(q_estado *q_susp_ready,
                                            q_estado *q_ready,
                                            q_estado *q_exit);

#endif // PLANIFICADOR_MEDIANO_PLAZO_H
