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
#include "plani_corto_plazo.h"

void inicializar_planificador_largo_plazo(void);

void insertar_proceso_nuevo(char *pseudocodigo, u_int32_t tamanio_proceso);

void puede_admitir_proceso_nuevo(void);

void insertar_en_exit(t_pcb *proceso);

#endif // PLANIFICADOR_LARGO_PLAZO_H
