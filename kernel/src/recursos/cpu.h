#ifndef RECURSOS_CPU_H
#define RECURSOS_CPU_H

#include <stdlib.h>
#include <pthread.h>
#include <utils/sockets/sockets.h>
#include <utils/protocol/protocol.h>

#include "config/config.h"
#include "logger/logger.h"

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
void iniciar_cpu(void);
void *manejar_conexion_dispatch(void *fd_cpu);
void *manejar_conexion_interrupt(void *fd_cpu);

#endif // RECURSOS_CPU_H
