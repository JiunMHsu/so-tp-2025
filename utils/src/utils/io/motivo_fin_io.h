#ifndef UTILS_MOTIVO_FIN_IO_H
#define UTILS_MOTIVO_FIN_IO_H

#include <utils/protocol/protocol.h>

typedef enum
{
    EXECUTED,
    DISCONNECTED,
} motivo_fin_io;

void enviar_motivo_fin_io(int32_t fd_conexion, motivo_fin_io motivo);
motivo_fin_io recibir_motivo_fin_io(int32_t fd_conexion);

#endif // UTILS_MOTIVO_FIN_IO_H
