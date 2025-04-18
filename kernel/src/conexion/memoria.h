#ifndef CONEXION_MEMORIA_H
#define CONEXION_MEMORIA_H

#include <stdlib.h>
#include <pthread.h>
#include <commons/string.h>
#include <commons/config.h>
#include <utils/sockets/sockets.h>
#include <utils/protocol/protocol.h>

#include "config/config.h"
#include "logger/logger.h"

int32_t solicitar_creacion_proceso(int32_t pid, int32_t tamanio, char *ruta_codigo);
int32_t solicitar_finalizacion_proceso(u_int32_t pid);
// probablemente solicitudes de swapping

#endif // CONEXION_MEMORIA_H