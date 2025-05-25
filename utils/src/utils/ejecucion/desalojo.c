#include "desalojo.h"

static t_packet *empaquetar_desalojo(t_desalojo *desalojo);

t_desalojo *crear_desalojo(u_int32_t pid, u_int32_t program_counter, motivo_desalojo motivo, char *syscall)
{
    t_desalojo *desalojo = malloc(sizeof(t_desalojo));
    
    desalojo->pid = pid;
    desalojo->program_counter = program_counter;
    desalojo->motivo = motivo;
    desalojo->syscall = syscall; //TODO: revisar cuando se libera, creo que pasa en kernel

    return desalojo;
}

void destruir_desalojo(t_desalojo *desalojo)
{
    free(desalojo); //TODO: no se si solo se libera la direccion de desalojo o si se debe liberar char *syscall tambien
}

void enviar_desalojo(int32_t fd_conexion, t_desalojo *desalojo)
{
    t_packet *paquete = empaquetar_desalojo(desalojo);
    enviar_paquete(paquete, fd_conexion);
}

// TODO: implementar recibir_desalojo
t_desalojo *recibir_desalojo(int32_t fd_conexion)
{
    return NULL;
}

static t_packet *empaquetar_desalojo(t_desalojo *desalojo)
{
    t_packet *paquete = crear_paquete();
    
    agregar_a_paquete(paquete, desalojo, sizeof(t_desalojo));

    return paquete;
}
