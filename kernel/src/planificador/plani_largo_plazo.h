#ifndef PLANIFICADOR_LARGO_PLAZO_H
#define PLANIFICADOR_LARGO_PLAZO_H

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#include "config/config.h"
#include "logger/logger.h"
#include "pcb/pcb.h"
#include "planificador.h"
#include "memoria.h"


#endif // PLANIFICADOR_LARGO_PLAZO_H

typedef emun
{
    FIFO,
    SJF
}algoritmos_de_planificacion;


void crear_procesos();

void esperar_solicitud(Proceso* proceso_nuevo);
