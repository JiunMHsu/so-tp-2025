#include "peticion_io.h"

static t_packet *empaquetar_peticion_io(t_peticion_io *peticion_io);

t_peticion_io *crear_peticion_io(u_int32_t pid, u_int32_t tiempo)
{
    t_peticion_io *peticion_io = malloc(sizeof(t_peticion_io));
    peticion_io->pid = pid;
    peticion_io->tiempo = tiempo;

    return peticion_io;
}

void destruir_peticion_io(t_peticion_io *peticion_io)
{
    if (peticion_io == NULL)
        return;

    free(peticion_io);
    peticion_io = NULL;
}

void enviar_peticion_io(int32_t fd_conexion, t_peticion_io *peticion_io)
{
    t_packet *paquete = empaquetar_peticion_io(peticion_io);
    enviar_paquete(paquete, fd_conexion);
    eliminar_paquete(paquete);
}

t_peticion_io *recibir_peticion_io(int32_t fd_conexion)
{
    t_list *paquete = recibir_paquete(fd_conexion);
    if (paquete == NULL)
        return NULL;

    t_peticion_io *peticion_io = malloc(sizeof(t_peticion_io));
    peticion_io->pid = *((u_int32_t *)list_get(paquete, 0));
    peticion_io->tiempo = *((u_int32_t *)list_get(paquete, 1));

    list_destroy_and_destroy_elements(paquete, &free);
    return peticion_io;
}

static t_packet *empaquetar_peticion_io(t_peticion_io *peticion_io)
{
    t_packet *paquete = crear_paquete();
    agregar_a_paquete(paquete, &peticion_io->pid, sizeof(u_int32_t));
    agregar_a_paquete(paquete, &peticion_io->tiempo, sizeof(u_int32_t));

    return paquete;
}
