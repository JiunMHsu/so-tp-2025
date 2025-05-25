#ifndef PLANIFICADOR_H
#define PLANIFICADOR_H

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#include "config/config.h"
#include "estado/estado.h"
#include "pcb/pcb.h"
#include "plani_largo_plazo.h"
#include "plani_mediano_plazo.h"
#include "plani_corto_plazo.h"

/**
 * @brief Inicializa el planificador del kernel recibiendo primer ejecutable.
 * Crea el proceso inicial y se bloquea esperando una entrada por teclado (ENTER)
 * para empezar a planificar.
 *
 * @note Crea los estados.
 * @note Crea los semaforos necesarios.
 * @note Lanza las rutinas de planificacion (crea los hilos).
 */

void inicializar_planificador(char *archivo_pseudocodigo, u_int32_t tamanio_proceso);

#endif // PLANIFICADOR_H
