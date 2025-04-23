#include "desalojo.h"

static t_packet *empaquetar_desalojo(t_desalojo *desalojo);

// TODO: implementar crear_desalojo
t_desalojo *crear_desalojo(u_int32_t pid, u_int32_t program_counter, motivo_desalojo motivo, char *syscall)
{
    return NULL;
}

// TODO: implementar destruir_desalojo
void destruir_desalojo(t_desalojo *desalojo)
{
}

// TODO: implementar enviar_desalojo
void enviar_desalojo(int32_t fd_conexion, t_desalojo *desalojo)
{
}

// TODO: implementar recibir_desalojo
t_desalojo *recibir_desalojo(int32_t fd_conexion)
{
    return NULL;
}

// TODO: implementar empaquetar_desalojo
static t_packet *empaquetar_desalojo(t_desalojo *desalojo)
{
    return NULL;
}
