#ifndef UTILS_DESALOJO_H
#define UTILS_DESALOJO_H

#include <stdlib.h>
#include <stdint.h>
#include <commons/string.h>
#include <utils/sockets/sockets.h>
#include <utils/protocol/protocol.h>

typedef enum
{
    SCHEDULER_INT,
    SYSCALL,
} motivo_desalojo;

typedef struct
{
    u_int32_t pid;
    u_int32_t program_counter;
    motivo_desalojo motivo;
    char *syscall;
} t_desalojo;

t_desalojo *crear_desalojo(u_int32_t pid, u_int32_t program_counter, motivo_desalojo motivo, char *syscall);
void destruir_desalojo(t_desalojo *desalojo);
void enviar_desalojo(int32_t fd_conexion, t_desalojo *desalojo);
t_desalojo *recibir_desalojo(int32_t fd_conexion);

#endif // UTILS_DESALOJO_H
