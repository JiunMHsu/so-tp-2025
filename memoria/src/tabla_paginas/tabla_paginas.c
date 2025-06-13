#include "tabla_paginas.h"

t_proceso_memoria *crear_proceso_memoria()
{
    t_proceso_memoria *proceso = malloc(sizeof(t_proceso_memoria));

    proceso->tabla_global = crear_tabla_de_paginas(0, get_cantidad_niveles(), get_entradas_por_tabla());
    proceso->accesos_tablas = 0;
    proceso->instrucciones_solicitadas = 0;
    proceso->paginas_en_swap = 0;
    proceso->paginas_en_memoria = 0;
    proceso->lecturas_mem = 0;
    proceso->escrituras_mem = 0;

    return proceso;
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

        if (nivel_tabla_actual < entradas_por_tabla - 1)
        {
            entrada->siguiente_nivel = crear_tabla_de_paginas(nivel_tabla_actual + 1, nivel_total_tablas, entradas_por_tabla);
        }
        list_add(tabla->entradas, entrada);
    }

    return tabla;
}

void destruir_tabla_de_paginas_para_proceso(t_proceso_tabla *tabla, int32_t entradas_por_tabla)
{
    for (int i = 0; i < entradas_por_tabla; i++)
    {
        if (tabla->entradas[i]->siguiente_nivel != NULL)
        {
            destruir_tabla_de_paginas_para_proceso(tabla->entradas[i]->siguiente_nivel, entradas_por_tabla);
        }

        free(tabla->entradas[i]);
    }
    free(tabla->entradas);
    free(tabla);
}

//TODO MIIIINNGGGGG!!!
void destruir_entrada(void *entrada_liberar)
{
    t_entrada_tabla *entrada = (t_entrada_tabla *) entrada_liberar;
    free()
}