#ifndef IO_CONEXION_KERNEL_H
#define IO_CONEXION_KERNEL_H

#include <stdlib.h>
#include <stdint.h>
#include <utils/sockets/sockets.h>
#include <utils/protocol/protocol.h>

#include "config/config.h"

int32_t conectar_con_kernel(char *nombre_interfaz);

#endif // IO_CONEXION_KERNEL_H
