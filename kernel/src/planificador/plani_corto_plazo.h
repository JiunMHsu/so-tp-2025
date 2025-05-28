#ifndef PLANIFICADOR_CORTO_PLAZO_H
#define PLANIFICADOR_CORTO_PLAZO_H

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#include "config/config.h"
#include "logger/logger.h"
#include "estado/estado.h"
#include "pcb/pcb.h"
#include "recursos/cpu.h"

void inicializar_planificador_corto_plazo(q_estado *q_ready);

#endif // PLANIFICADOR_CORTO_PLAZO_H
