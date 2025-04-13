#ifndef CPU_CLIENTE_H
#define CPU_CLIENTE_H

#include <stdlib.h>
#include <commons/string.h>
#include <commons/config.h>
#include <utils/sockets/sockets.h>
#include <utils/protocol/protocol.h>
#include "config/config.h"

void iniciar_cliente(void);
void finalizar_cliente(void);

#endif