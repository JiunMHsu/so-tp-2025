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

#endif // PLANIFICADOR_MEDIANO_PLAZO_H
