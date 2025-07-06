#ifndef TABLA_PAGINAS_H
#define TABLA_PAGINAS_H

#include <stdlib.h>

#include "config/config.h"

typedef struct
{
    int32_t nivel_de_tabla;
    t_list *entradas;
    // t_entrada_tabla **entradas; // TODO Remplazar por listas de entradas

} t_proceso_tabla;

typedef struct
{
    int32_t presente; // 1 si está cargada, 0 si no

    // Si no es el último nivel
    t_proceso_tabla *siguiente_nivel;

    // Si es el último nivel
    int32_t marco; // -1 si no tiene asignado
} t_entrada_tabla;



t_proceso_memoria *crear_proceso_memoria();
t_proceso_tabla *crear_tabla_de_paginas(int32_t nivel_tabla_actual, int32_t nivel_total_tablas, int32_t entradas_por_tabla);
void destruir_tabla_de_paginas_para_proceso(t_proceso_tabla *tabla);
void destruir_entrada(void *entrada_liberar);

#endif // TABLA_PAGINAS_H