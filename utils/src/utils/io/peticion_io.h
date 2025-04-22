#ifndef UTILS_PETICION_IO_H
#define UTILS_PETICION_IO_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <utils/protocol/protocol.h>

typedef struct
{
    u_int32_t pid;
    u_int32_t tiempo;
} t_peticion_io;

t_peticion_io *crear_peticion_io(u_int32_t pid, u_int32_t tiempo);
void destruir_peticion_io(t_peticion_io *peticion_io);
void enviar_peticion_io(int32_t fd_conexion, t_peticion_io *peticion_io);
t_peticion_io *recibir_peticion_io(int32_t fd_conexion);

#endif // UTILS_PETICION_IO_H
