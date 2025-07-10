#include "mem_datos_paginacion.h"

t_mem_datos_paginacion *crear_mem_datos_paginacion(u_int32_t tamanio_pagina, u_int32_t cantidad_entradas_tp, u_int32_t cantidad_niveles)
{
    t_mem_datos_paginacion *mem_datos_paginacion = malloc(sizeof(t_mem_datos_paginacion));
    mem_datos_paginacion->tamanio_pagina = tamanio_pagina;
    mem_datos_paginacion->cantidad_entradas_tp = cantidad_entradas_tp;
    mem_datos_paginacion->cantidad_niveles = cantidad_niveles;

    return mem_datos_paginacion;
}

t_packet *serializar_mem_datos_paginacion(t_mem_datos_paginacion *mem_datos_paginacion)
{
    t_packet *paquete = crear_paquete();
    agregar_a_paquete(paquete, mem_datos_paginacion->cantidad_entradas_tp, sizeof(u_int32_t));
    agregar_a_paquete(paquete, mem_datos_paginacion->cantidad_niveles, sizeof(u_int32_t));
    agregar_a_paquete(paquete, mem_datos_paginacion->tamanio_pagina, sizeof(u_int32_t));

    return paquete;
}

void enviar_mem_datos_paginacion(u_int32_t fd_conexion, t_mem_datos_paginacion *mem_datos_paginacion)
{
    t_packet *paquete = serializar_mem_datos_paginacion(mem_datos_paginacion);
    enviar_paquete(paquete, fd_conexion);
    eliminar_paquete(paquete);
}

t_mem_datos_paginacion *recibir_mem_datos_paginacion(u_int32_t fd_conexion)
{
    t_list *paquete = recibir_paquete(fd_conexion);
    if (paquete == NULL)
        return NULL;

    t_mem_datos_paginacion *mem_datos_paginacion = malloc(sizeof(t_mem_datos_paginacion));
    mem_datos_paginacion->cantidad_entradas_tp = *(u_int32_t *)list_get(paquete, 0);
    mem_datos_paginacion->cantidad_niveles = *(u_int32_t *)list_get(paquete, 1);
    mem_datos_paginacion->tamanio_pagina = *(u_int32_t *)list_get(paquete, 2);

    list_destroy(paquete);
    return mem_datos_paginacion;
}

void destruir_mem_datos_paginacion(t_mem_datos_paginacion *mem_datos_paginacion)
{
    free(mem_datos_paginacion);
}
