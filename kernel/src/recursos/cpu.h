#ifndef RECURSOS_CPU_H
#define RECURSOS_CPU_H

#include <stdlib.h>
#include <pthread.h>
#include <commons/collections/list.h>  // temporal (reemplazar por mlist)
#include <commons/collections/queue.h> // temporal (reemplazar por mqueue)
#include <utils/sockets/sockets.h>
#include <utils/protocol/protocol.h>
// #include <utils/mlist/mlist.h>
// #include <utils/mqueue/mqueue.h>

#include "config/config.h"
#include "logger/logger.h"

typedef struct
{
    char *id;
    int32_t fd_dispatch;
    int32_t fd_interrupt;

    /**
     * @brief PID del proceso que está ejecutando la CPU.
     *
     * @note -1 indica que no está ejecutando nada.
     *
     */
    int32_t executing;

    // capaz necesita algunos semaforos para sincro
} t_cpu;

typedef struct
{
    u_int32_t pid;
    u_int32_t program_counter;
    int32_t fd_dispatch;
} args_ejecucion;

typedef enum
{
    SCHEDULER_INT,
    SYSCALL,
} motivo_desalojo;

typedef struct
{
    u_int32_t pid;
    u_int32_t program_counter;
    motivo_desalojo motivo;
    char *syscall;
} t_desalojo;

/**
 * @brief Inicializa la colección global para manejar las CPUs.
 *
 */
void inicializar_cpu(void);
// void destruir_cpu(void);

void conectar_cpu(char *id_cpu, int32_t fd_dispatch, int32_t fd_interrupt);

/**
 * @brief Indica la ejecución de un proceso sobre una CPU.
 *
 * @param pid
 * @param program_counter
 *
 * @return -1 si no se pudo ejecutar el proceso. 0 en caso de éxito.
 *
 * @note No es y no debería ser bloqueante.
 * @note Asume que hay al menos una CPU disponible.
 *
 */
int8_t ejecutar(u_int32_t pid, u_int32_t program_counter);

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
 * @return t_desalojo*
 *
 * @note Es bloqueante.
 * @note Se debe liberar la estructura retornada con `destruir_desalojo`.
 */
t_desalojo *get_desalojo(void);

void destruir_desalojo(t_desalojo *desalojado);

#endif // RECURSOS_CPU_H
