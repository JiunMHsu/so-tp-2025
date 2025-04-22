#ifndef CONEXION_KERNEL_H
#define CONEXION_KERNEL_H

#include <stdlib.h>
#include <pthread.h>
#include <commons/string.h>
#include <commons/config.h>
#include <utils/sockets/sockets.h>
#include <utils/protocol/protocol.h>
#include <utils/ejecucion/peticion_ejecucion.h>
#include <utils/ejecucion/desalojo.h>

#include "config/config.h"
#include "logger/logger.h"

typedef struct
{
    int32_t fd_dispatch;
    int32_t fd_interrupt;
} t_kernel_sockets;

t_kernel_sockets conectar_kernel(void);

// recibir_peticion_ejecucion
// recibir_interrupt

#endif // CONEXION_KERNEL_H
