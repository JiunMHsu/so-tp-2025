#ifndef RECURSOS_IO_H
#define RECURSOS_IO_H

#include <stdlib.h>
#include <pthread.h>
#include <commons/collections/list.h>  // temporal (reemplazar por mlist)
#include <commons/collections/queue.h> // temporal (reemplazar por mqueue)
#include <commons/collections/dictionary.h>
#include <utils/sockets/sockets.h>
#include <utils/protocol/protocol.h>
// #include <utils/mlist/mlist.h>
// #include <utils/mqueue/mqueue.h>

#include "config/config.h"
#include "logger/logger.h"

typedef struct
{
    char *nombre;
    int32_t fd_io;
} t_io;

/**
 * @brief Inicializa la colección global para manejar los IOs.
 *
 */
void inicializar_io(void);
void conectar_io(char *nombre, int32_t fd_io);

#endif // RECURSOS_IO_H
