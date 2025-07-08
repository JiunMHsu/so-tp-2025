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

void cargar_marcos_asignados(u_int32_t pid, t_list *frames_asignados)
{
    t_tabla *tabla_raiz = buscar_por_pid(pid);

    t_list *entradas_ultimo_nivel = listar_entradas_ultimo_nivel(tabla_raiz);

    for (int i = 0; i < list_size(entradas_ultimo_nivel); i++)
    {
        t_entrada *entrada = list_get(entradas_hoja, i);
        u_int32_t *marco = list_get(frames_asignados, i);

        entrada->presente = 1;
        entrada->marco = *marco;
    }

    list_destroy(entradas_ultimo_nivel);
}

int32_t obtener_marco(u_int32_t pid, u_int32_t *paginas)
{
    t_tabla *tabla = buscar_por_pid(pid);
    int32_t marco = -1;

    for (int nivel = 0; nivel < get_cantidad_niveles(); nivel++)
    {
        t_entrada *entrada = list_get(tabla->entradas, paginas[nivel]);

        if (entrada->siguiente == NULL)
            return entrada->marco;

        tabla = entrada->siguiente;
    }

    return marco;
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

t_list *listar_entradas_ultimo_nivel(t_tabla *tabla_raiz)
{
    t_list *lista_entradas;
    recorrer_tablas(lista_entradas, tabla_raiz, 1);
    return lista_entradas;
}

void recorrer_tablas(t_list *lista_entradas, t_tabla *tabla, u_int32_t nivel)
{
    u_int32_t cantidad_niveles = get_cantidad_niveles();
    u_int32_t entradas_por_tabla = get_entradas_por_tabla();

    for (int i = 0; i < entradas_por_tabla; i++)
    {
        t_entrada *entrada = list_get(tabla->entradas, i);

        if (nivel < cantidad_niveles)
        {
            recorrer_tablas(lista_entradas, entrada->siguiente, nivel + 1);
        }

        list_add(lista_entradas, entrada);
    }
}