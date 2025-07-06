#include "tabla_paginas.h"

void inicializar_tabla_de_paginas()
{
}

void crear_proceso_memoria()
{
}

t_proceso_tabla *crear_tabla_de_paginas(int32_t nivel_tabla_actual, int32_t nivel_total_tablas, int32_t entradas_por_tabla)
{
    t_proceso_tabla *tabla = malloc(sizeof(t_proceso_tabla));
    tabla->nivel_de_tabla = nivel_tabla_actual;
    tabla->entradas = list_create();

    for (int i = 0; i < entradas_por_tabla; i++)
    {
        t_entrada_tabla *entrada = malloc(sizeof(t_entrada_tabla));
        entrada->presente = 0;
        entrada->marco = -1;
        entrada->siguiente_nivel = NULL;

        if (nivel_tabla_actual < nivel_total_tablas - 1)
        {
            entrada->siguiente_nivel = crear_tabla_de_paginas(nivel_tabla_actual + 1, nivel_total_tablas, entradas_por_tabla);
        }
        list_add(tabla->entradas, entrada);
    }

    return tabla;
}

void destruir_tabla_de_paginas_para_proceso(t_proceso_tabla *tabla)
{
    list_destroy_and_destroy_elements(tabla->entradas, destruir_entrada);

    free(tabla);
}

void destruir_entrada(void *entrada_liberar)
{
    t_entrada_tabla *entrada = (t_entrada_tabla *)entrada_liberar;

    if (entrada->siguiente_nivel != NULL)
    {
        destruir_tabla_de_paginas_para_proceso(entrada->siguiente_nivel);
    }

    free(entrada);
}