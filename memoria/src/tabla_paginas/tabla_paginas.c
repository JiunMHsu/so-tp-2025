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
static t_list *get_entradas_ultimo_nivel(t_tabla *tabla_raiz);
static void recorrer_tablas(t_list *lista_entradas, t_tabla *tabla, u_int32_t nivel);

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
    free(_pid);
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
    t_list *entradas_ultimo_nivel = get_entradas_ultimo_nivel(tabla_raiz);

    // se evalúa por tamaño de frames asignados
    // se supone que la cantidad de frames asignados serán siempre menor
    // o igual a la cantidad de entradas del último nivel (cantidad de marcos direccionables)
    for (int i = 0; i < list_size(frames_asignados); i++)
    {
        t_entrada *entrada = list_get(entradas_ultimo_nivel, i);

        entrada->presente = 1;
        entrada->marco = *(u_int32_t *)list_get(frames_asignados, i);
    }

    list_destroy(entradas_ultimo_nivel);
}

int32_t obtener_marco(u_int32_t pid, t_list *entradas)
{
    int32_t marco = -2; // por un tema de como se maneja la señal (-1 es error)
    t_tabla *tabla = buscar_por_pid(pid);
    if (!tabla)
        return marco;

    u_int32_t cantidad_niveles = get_cantidad_niveles();

    for (int nivel = 0; nivel < cantidad_niveles; nivel++)
    {
        u_int32_t nro_entrada = *(u_int32_t *)list_get(entradas, nivel);
        t_entrada *entrada = list_get(tabla->entradas, nro_entrada);

        incrementar_acceso_tabla(pid);

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

t_list *obtener_marcos_asignados(u_int32_t pid)
{
    t_tabla *tabla = buscar_por_pid(pid);
    if (tabla == NULL)
        return NULL;

    t_list *marcos_asignados = list_create();

    pthread_mutex_lock(&mutex_tablas_procesos);
    t_list *entradas_ultimo_nivel = get_entradas_ultimo_nivel(tabla);

    for (int i = 0; i < list_size(entradas_ultimo_nivel); i++)
    {
        t_entrada *entrada = list_get(entradas_ultimo_nivel, i);
        if (!entrada->presente)
            continue; // Solo agregar entradas presentes

        u_int32_t *marco = malloc(sizeof(u_int32_t));
        *marco = entrada->marco;
        list_add(marcos_asignados, marco);
    }

    list_destroy(entradas_ultimo_nivel);
    pthread_mutex_unlock(&mutex_tablas_procesos);

    return marcos_asignados;
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

static t_list *get_entradas_ultimo_nivel(t_tabla *tabla_raiz)
{
    t_list *lista_entradas = list_create();
    recorrer_tablas(lista_entradas, tabla_raiz, 1);
    return lista_entradas;
}

static void recorrer_tablas(t_list *lista_entradas, t_tabla *tabla, u_int32_t nivel)
{
    u_int32_t entradas_por_tabla = get_entradas_por_tabla();

    for (int i = 0; i < entradas_por_tabla; i++)
    {
        t_entrada *entrada = list_get(tabla->entradas, i);
        if (entrada->siguiente == NULL)
        {
            list_add(lista_entradas, entrada);
        }
        else
        {
            recorrer_tablas(lista_entradas, entrada->siguiente, nivel + 1);
        }
    }
}