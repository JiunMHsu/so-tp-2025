#ifndef MEM_DATOS_PAGINACION
#define MEM_DATOS_PAGINACION

#include <utils/protocol/protocol.h>

typedef struct
{
    u_int32_t tamanio_pagina;
    u_int32_t cantidad_entradas_tp;
    u_int32_t cantidad_niveles;
} t_mem_datos_paginacion;

t_mem_datos_paginacion *crear_mem_datos_paginacion(u_int32_t tamanio_pagina, u_int32_t cantidad_entradas_tp, u_int32_t cantidad_niveles);
void enviar_mem_datos_paginacion(u_int32_t fd_conexion, t_mem_datos_paginacion *mem_datos_paginacion);
t_mem_datos_paginacion *recibir_mem_datos_paginacion(u_int32_t fd_conexion);
void destruir_mem_datos_paginacion(t_mem_datos_paginacion *mem_datos_paginacion);

#endif // MEM_DATOS_PAGINACION