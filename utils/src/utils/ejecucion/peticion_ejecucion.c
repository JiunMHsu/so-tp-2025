#include "peticion_ejecucion.h"

static t_packet *empaquetar_peticion_ejecucion(t_peticion_ejecucion *peticion);

t_peticion_ejecucion *crear_peticion_ejecucion(u_int32_t pid, u_int32_t program_counter)
{
    t_peticion_ejecucion *peticion = malloc(sizeof(t_peticion_ejecucion));
    peticion->pid = pid;
    peticion->program_counter = program_counter;

    return peticion;
}

void destruir_peticion_ejecucion(t_peticion_ejecucion *peticion)
{
    if (peticion == NULL)
        return;

    free(peticion);
    peticion = NULL;
}

void enviar_peticion_ejecucion(int32_t fd_dispatch, t_peticion_ejecucion *peticion)
{
    t_packet *packet = empaquetar_peticion_ejecucion(peticion);
    enviar_paquete(fd_dispatch, packet);
    eliminar_paquete(packet);
}

t_peticion_ejecucion *recibir_peticion_ejecucion(int32_t fd_dispatch)
{
    t_list *paquete = recibir_paquete(fd_dispatch);
    if (paquete == NULL)
        return NULL;

    t_peticion_ejecucion *peticion = malloc(sizeof(t_peticion_ejecucion));
    peticion->pid = *((u_int32_t *)list_get(paquete, 0));
    peticion->program_counter = *((u_int32_t *)list_get(paquete, 1));

    list_destroy_and_destroy_elements(paquete, &free);
    return peticion;
}

static t_packet *empaquetar_peticion_ejecucion(t_peticion_ejecucion *peticion)
{
    t_packet *paquete = crear_paquete();
    agregar_a_paquete(paquete, &(peticion->pid), sizeof(u_int32_t));
    agregar_a_paquete(paquete, &(peticion->program_counter), sizeof(u_int32_t));

    return paquete;
}
