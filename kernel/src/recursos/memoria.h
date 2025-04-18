#ifndef RECURSOS_MEMORIA_H
#define RECURSOS_MEMORIA_H

#include <stdlib.h>
#include <pthread.h>
#include <utils/sockets/sockets.h>
#include <utils/protocol/protocol.h>

#include "config/config.h"
#include "logger/logger.h"

int32_t solicitar_creacion_proceso(u_int32_t pid, u_int32_t tamanio, char *ruta_codigo);
int32_t solicitar_finalizacion_proceso(u_int32_t pid);
// probablemente solicitudes de swapping

#endif // RECURSOS_MEMORIA_H