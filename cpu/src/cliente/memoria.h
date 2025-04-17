#ifndef CLIENTE_MEMORIA_H
#define CLIENTE_MEMORIA_H

#include <stdlib.h>
#include <pthread.h>
#include <commons/string.h>
#include <commons/config.h>
#include <utils/sockets/sockets.h>
#include <utils/protocol/protocol.h>

#include "config/config.h"
#include "logger/logger.h"

int32_t conectar_memoria(void);
void *atender_memoria(void *);

#endif