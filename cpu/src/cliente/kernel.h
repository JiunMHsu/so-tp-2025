#ifndef CLIENTE_KERNEL_H
#define CLIENTE_KERNEL_H

#include <stdlib.h>
#include <commons/string.h>
#include <commons/config.h>
#include <utils/sockets/sockets.h>
#include <utils/protocol/protocol.h>
#include "config/config.h"

uint32_t iniciar_conexion_kernel(void);
void finalizar_cliente(void);

#endif