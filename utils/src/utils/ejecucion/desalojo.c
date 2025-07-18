#include "desalojo.h"

static t_packet *empaquetar_desalojo(t_desalojo *desalojo);

t_desalojo *crear_desalojo(u_int32_t pid, u_int32_t program_counter, motivo_desalojo motivo, char *syscall)
{
    t_desalojo *desalojo = malloc(sizeof(t_desalojo));

    desalojo->pid = pid;
    desalojo->program_counter = program_counter;
    desalojo->motivo = motivo;
    desalojo->syscall = syscall ? strdup(syscall) : NULL;

    return desalojo;
}

void destruir_desalojo(t_desalojo *desalojo)
{
    if (desalojo == NULL)
        return;

    if (desalojo->syscall != NULL)
        free(desalojo->syscall);

    free(desalojo);
    desalojo = NULL;
}

void enviar_desalojo(int32_t fd_conexion, t_desalojo *desalojo)
{
    t_packet *paquete = empaquetar_desalojo(desalojo);
    enviar_paquete(paquete, fd_conexion);
    eliminar_paquete(paquete);
}

t_desalojo *recibir_desalojo(int32_t fd_conexion)
{
    t_list *paquete = recibir_paquete(fd_conexion);
    if (paquete == NULL)
        return NULL;

    t_desalojo *desalojo = malloc(sizeof(t_desalojo));
    desalojo->pid = *((u_int32_t *)list_get(paquete, 0));
    desalojo->program_counter = *((u_int32_t *)list_get(paquete, 1));
    desalojo->motivo = *((motivo_desalojo *)list_get(paquete, 2));

    if (desalojo->motivo == SYSCALL)
        desalojo->syscall = strdup((char *)list_get(paquete, 3));
    else
        desalojo->syscall = NULL;

    list_destroy_and_destroy_elements(paquete, &free);
    return desalojo;
}

static t_packet *empaquetar_desalojo(t_desalojo *desalojo)
{
    t_packet *paquete = crear_paquete();

    agregar_a_paquete(paquete, &(desalojo->pid), sizeof(u_int32_t));
    agregar_a_paquete(paquete, &(desalojo->program_counter), sizeof(u_int32_t));
    agregar_a_paquete(paquete, &(desalojo->motivo), sizeof(motivo_desalojo));

    if (desalojo->motivo == SYSCALL)
        agregar_a_paquete(paquete, desalojo->syscall, string_full_length(desalojo->syscall));

    return paquete;
}
