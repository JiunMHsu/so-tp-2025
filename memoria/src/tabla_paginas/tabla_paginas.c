#include "tabla_paginas.h"

static t_dictionary *tablas_procesos;
static pthread_mutex_t mutex_tablas_procesos;

/**
 * @brief Crea tablas multinivel para un proceso.
 * Iniciando por `nivel` y continuando hasta `cant_niveles`.
 *
 * @return `t_tabla*` primer nivel de la tabla de páginas.
 */
static t_tabla *crear_tabla_paginas(u_int32_t nivel, u_int32_t cant_niveles);
static t_entrada *crear_entrada(void);

static t_tabla *buscar_por_pid(u_int32_t pid);

void inicializar_tabla_de_paginas()
{
    tablas_procesos = dictionary_create();
    pthread_mutex_init(&mutex_tablas_procesos, NULL);
}

void crear_tablas_para(u_int32_t pid)
{
    char *_pid = string_itoa(pid);
    pthread_mutex_lock(&mutex_tablas_procesos);
    if (dictionary_has_key(tablas_procesos, _pid))
    {
        pthread_mutex_unlock(&mutex_tablas_procesos);
        free(_pid);
        return;
    }

    t_tabla *tablas = crear_tabla_paginas(1, get_cantidad_niveles());
    dictionary_put(tablas_procesos, _pid, tablas);

    pthread_mutex_unlock(&mutex_tablas_procesos);
}

static t_tabla *crear_tabla_paginas(u_int32_t nivel, u_int32_t cant_niveles)
{
    t_tabla *tabla = malloc(sizeof(t_tabla));
    tabla->nivel = nivel;
    tabla->entradas = list_create();

    u_int32_t cant_entradas = get_entradas_por_tabla();
    for (int i = 0; i < cant_entradas; i++)
    {
        t_entrada *entrada = crear_entrada();

        if (nivel < cant_niveles)
            entrada->siguiente = crear_tabla_paginas(nivel + 1, cant_niveles);

        list_add(tabla->entradas, entrada);
    }

    return tabla;
}

static t_entrada *crear_entrada()
{
    t_entrada *entrada = malloc(sizeof(t_entrada));
    entrada->presente = 0;
    entrada->marco = -1;
    entrada->siguiente = NULL;

    return entrada;
}

void destruir_tablas_para(u_int32_t pid) {}

// void destruir_tabla_de_paginas_para_proceso(t_proceso_tabla *tabla)
// {
//     list_destroy_and_destroy_elements(tabla->entradas, destruir_entrada);

//     free(tabla);
// }

// void destruir_entrada(void *entrada_liberar)
// {
//     t_entrada_tabla *entrada = (t_entrada_tabla *)entrada_liberar;

//     if (entrada->siguiente_nivel != NULL)
//     {
//         destruir_tabla_de_paginas_para_proceso(entrada->siguiente_nivel);
//     }

//     free(entrada);
// }
