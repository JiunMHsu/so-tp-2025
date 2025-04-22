#include "motivo_fin_io.h"

void enviar_motivo_fin_io(int32_t fd_conexion, motivo_fin_io motivo)
{
    enviar_senial((int32_t)motivo, fd_conexion);
}

motivo_fin_io recibir_motivo_fin_io(int32_t fd_conexion)
{
    int32_t motivo = recibir_senial(fd_conexion);
    if (motivo == -1)
        return DISCONNECTED;

    return (motivo_fin_io)motivo;
}
