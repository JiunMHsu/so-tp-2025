#ifndef TABLA_PAGINAS_H
#define TABLA_PAGINAS_H

#include <stdlib.h>

typedef struct
{
    int32_t presente; // 1 si está cargada, 0 si no

    // Si no es el último nivel
    struct t_proceso_tabla *siguiente_nivel;

    // Si es el último nivel
    int32_t marco; // -1 si no tiene asignado
} t_entrada_tabla;

typedef struct
{
    t_entrada_tabla **entradas; // array dinámico de entradas

} t_proceso_tabla;

typedef struct
{
    t_proceso_tabla *tabla; // índice = ID tabla

} t_proceso_memoria;

t_proceso_tabla *crear_tabla_de_paginas();
void inicializar_tabla_paginas(t_proceso_tabla*); // no se si va 
void destruir_tabla_de_paginas_para_proceso(int32_t pid);

#endif // TABLA_PAGINAS_H