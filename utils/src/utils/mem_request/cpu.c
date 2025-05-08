#include "cpu.h"

t_peticion_cpu *recibir_peticion_cpu(int32_t fd_conexion)
{
    //escuchar
    t_list* paquete = recibir_paquete(fd_conexion);
    t_peticion_cpu* peticion = malloc(sizeof(t_peticion_cpu));


    peticion->pid = *(u_int32_t*) list_get(paquete, 0);
    peticion->program_counter = *(u_int32_t*) list_get(paquete, 1);

    list_destroy_and_destroy_elements(paquete, &free);

    return peticion;
}

void destruir_peticion_cpu(t_peticion_cpu* peticion)
{
    free(peticion);
}
