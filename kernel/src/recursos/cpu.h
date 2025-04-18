#ifndef RECURSOS_CPU_H
#define RECURSOS_CPU_H

#include <stdlib.h>
#include <pthread.h>
#include <utils/sockets/sockets.h>
#include <utils/protocol/protocol.h>

#include "config/config.h"
#include "logger/logger.h"

/**
 * @brief Inicializa la colección global para manejar las CPUs.
 *
 */
void iniciar_cpu(void);
void *manejar_conexion_dispatch(void *fd_cpu);
void *manejar_conexion_interrupt(void *fd_cpu);

#endif // RECURSOS_CPU_H
