#ifndef CONEXION_MEMORIA_H
#define CONEXION_MEMORIA_H

#include <stdlib.h>
#include <pthread.h>
#include <utils/sockets/sockets.h>
#include <utils/protocol/protocol.h>

#include "config/config.h"
#include "logger/logger.h"

int8_t conectar_memoria(void);

// funciones de peticion a memoria (consumir utils/mem_request/cpu)

#endif // CONEXION_MEMORIA_H
