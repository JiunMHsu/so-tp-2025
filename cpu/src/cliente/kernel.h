#ifndef CLIENTE_KERNEL_H
#define CLIENTE_KERNEL_H

#include <stdlib.h>
#include <commons/string.h>
#include <commons/config.h>
#include <utils/sockets/sockets.h>
#include <utils/protocol/protocol.h>
#include "config/config.h"

int32_t conectar_kernel_dispatch(void);
int32_t conectar_kernel_interrupt(void);
void *atender_kernel_interrupt(void *);

#endif