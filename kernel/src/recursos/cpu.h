#ifndef RECURSOS_CPU_H
#define RECURSOS_CPU_H

#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <utils/protocol/protocol.h>
#include <utils/ejecucion/peticion_ejecucion.h>
#include <utils/ejecucion/desalojo.h>
#include <utils/mlist/mlist.h>
#include <utils/mqueue/mqueue.h>

#include "config/config.h"
#include "logger/logger.h"
#include "pcb/pcb.h"

typedef struct
{
    char *id;
    int32_t fd_dispatch;
    int32_t fd_interrupt;

    /**
     * @brief pcb del proceso que está ejecutando la CPU.
     *
     * @note NULL indica que no está ejecutando nada.
     *
     */
    t_pcb *proceso;
    pthread_mutex_t mutex_proceso;
    sem_t *hay_proceso;
} t_cpu;

typedef struct
{
    t_pcb *proceso;
    motivo_desalojo motivo;
    char *syscall;
} t_fin_de_ejecucion;

/**
 * @brief Inicializa la colección global para manejar las CPUs.
 *
 */
void inicializar_cpu(void);

void conectar_cpu(char *id_cpu, int32_t fd_dispatch, int32_t fd_interrupt);

/**
 * @brief Indica la ejecución de un proceso sobre una CPU.
 *
 * @param pid
 * @param program_counter
 *
 * @note Es bloqueante si no hay CPUs libres.
 */
void ejecutar(t_pcb *proceso);

/**
 * @brief Envía una interrupción a la CPU que se encuentra
 * ejecutando el proceso indicado.
 *
 * @param pid
 */
void enviar_interrupcion(u_int32_t pid);

/**
 * @brief Escucha constantemente si hay procesos desalojados de la CPU.
 * Sea por un syscall o por una interrupción.
 *
 * @return t_fin_de_ejecucion*
 *
 * @note Es bloqueante.
 * @note Se debe liberar la estructura retornada con `destruir_fin_de_ejecucion`.
 */
t_fin_de_ejecucion *get_fin_de_ejecucion(void);

/**
 * @brief Destruye una estructura de fin de ejecución.
 *
 * @param fin_de_ejecucion
 */
void destruir_fin_de_ejecucion(t_fin_de_ejecucion *fin_de_ejecucion);

#endif // RECURSOS_CPU_H
