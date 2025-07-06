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

static void destruir_tabla(t_tabla *tabla);
static void destruir_entrada(void *entrada);

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

u_int32_t obtener_marco(u_int32_t pid, u_int32_t *entradas)
{
    // t_tabla *tabla = buscar_por_pid(pid);
    // if (tabla == NULL)
    // {
    //     log_mensaje_error("No se encontró la tabla de páginas para el PID %u", pid);
    //     return -1;
    // }

    // if (entradas == NULL || list_size(tabla->entradas) < *entradas)
    // {
    //     log_mensaje_error("No hay suficientes entradas en la tabla de páginas para el PID %u", pid);
    //     return -1;
    // }

    // t_entrada *entrada = list_get(tabla->entradas, *entradas);
    // if (entrada == NULL || !entrada->presente)
    // {
    //     log_mensaje_error("La entrada no está presente o no es válida para el PID %u", pid);
    //     return -1;
    // }

    // return entrada->marco;
}

void destruir_tablas_para(u_int32_t pid)
{
    char *_pid = string_itoa(pid);
    pthread_mutex_lock(&mutex_tablas_procesos);

    if (!dictionary_has_key(tablas_procesos, _pid))
    {
        pthread_mutex_unlock(&mutex_tablas_procesos);
        free(_pid);
        return;
    }

    t_tabla *tabla = dictionary_remove(tablas_procesos, _pid);
    pthread_mutex_unlock(&mutex_tablas_procesos);

    destruir_tabla(tabla);
    free(_pid);
}

static void destruir_tabla(t_tabla *tabla)
{
    if (tabla == NULL)
        return;

    list_destroy_and_destroy_elements(tabla->entradas, &destruir_entrada);
    free(tabla);
    tabla = NULL;
}

static void destruir_entrada(void *entrada)
{
    if (entrada == NULL)
        return;

    t_entrada *_entrada = (t_entrada *)entrada;
    destruir_tabla(_entrada->siguiente); // NULL safe function
    free(entrada);
    entrada = NULL;
}

static t_tabla *buscar_por_pid(u_int32_t pid)
{
    char *_pid = string_itoa(pid);
    pthread_mutex_lock(&mutex_tablas_procesos);
    t_tabla *tabla = dictionary_get(tablas_procesos, _pid);
    pthread_mutex_unlock(&mutex_tablas_procesos);
    free(_pid);

    return tabla;
}
