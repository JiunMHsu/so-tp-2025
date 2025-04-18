#ifndef RECURSOS_IO_H
#define RECURSOS_IO_H

#include <stdlib.h>
#include <pthread.h>
#include <utils/sockets/sockets.h>
#include <utils/protocol/protocol.h>

#include "config/config.h"
#include "logger/logger.h"

/**
 * @brief Inicializa la colección global para manejar los IOs.
 *
 */
void iniciar_io(void);
void *manejar_conexion_io(void *fd_ptr);

#endif // RECURSOS_IO_H
