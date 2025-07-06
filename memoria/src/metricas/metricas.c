#include "metricas.h"

static t_dictionary *metricas_procesos;
static pthread_mutex_t mutex_metricas;

static t_metricas *crear_metricas(void);
static t_metricas *buscar_por_pid(u_int32_t pid);

void inicializar_metricas()
{
    metricas_procesos = dictionary_create();
    pthread_mutex_init(&mutex_metricas, NULL);
}

void crear_metricas_para(u_int32_t pid)
{
    char *_pid = string_itoa(pid);

    pthread_mutex_lock(&mutex_metricas);
    if (dictionary_has_key(metricas_procesos, _pid))
    {
        pthread_mutex_unlock(&mutex_metricas);
        free(_pid);
        return; // Ya existen métricas para este PID
    }

    t_metricas *metricas = crear_metricas();
    dictionary_put(metricas_procesos, _pid, metricas);

    pthread_mutex_unlock(&mutex_metricas);
}

t_metricas *remover_metricas_para(u_int32_t pid)
{
    char *_pid = string_itoa(pid);

    pthread_mutex_lock(&mutex_metricas);
    t_metricas *metricas = (t_metricas *)dictionary_remove(metricas_procesos, _pid);
    pthread_mutex_unlock(&mutex_metricas);

    free(_pid);
    return metricas;
}

void destruir_metricas(t_metricas *metricas)
{
    if (metricas == NULL)
        return;

    free(metricas);
    metricas = NULL;
}

void incrementar_acceso_tabla(u_int32_t pid)
{
    pthread_mutex_lock(&mutex_metricas);
    t_metricas *metricas = buscar_por_pid(pid);

    if (metricas != NULL)
        metricas->accesos_tablas++;

    pthread_mutex_unlock(&mutex_metricas);
}

void incrementar_instruccion_solicitada(u_int32_t pid)
{
    pthread_mutex_lock(&mutex_metricas);
    t_metricas *metricas = buscar_por_pid(pid);

    if (metricas != NULL)
        metricas->instrucciones_solicitadas++;

    pthread_mutex_unlock(&mutex_metricas);
}

void incrementar_swap_out(u_int32_t pid)
{
    pthread_mutex_lock(&mutex_metricas);
    t_metricas *metricas = buscar_por_pid(pid);

    if (metricas != NULL)
        metricas->swap_outs++;

    pthread_mutex_unlock(&mutex_metricas);
}

void incrementar_swap_in(u_int32_t pid)
{
    pthread_mutex_lock(&mutex_metricas);
    t_metricas *metricas = buscar_por_pid(pid);

    if (metricas != NULL)
        metricas->swap_ins++;

    pthread_mutex_unlock(&mutex_metricas);
}

void incrementar_lectura(u_int32_t pid)
{
    pthread_mutex_lock(&mutex_metricas);
    t_metricas *metricas = buscar_por_pid(pid);

    if (metricas != NULL)
        metricas->lecturas++;

    pthread_mutex_unlock(&mutex_metricas);
}

void incrementar_escritura(u_int32_t pid)
{
    pthread_mutex_lock(&mutex_metricas);
    t_metricas *metricas = buscar_por_pid(pid);

    if (metricas != NULL)
        metricas->escrituras++;

    pthread_mutex_unlock(&mutex_metricas);
}

static t_metricas *crear_metricas()
{
    t_metricas *metricas = malloc(sizeof(t_metricas));

    metricas->accesos_tablas = 0;
    metricas->instrucciones_solicitadas = 0;
    metricas->swap_outs = 0;
    metricas->swap_ins = 0;
    metricas->lecturas = 0;
    metricas->escrituras = 0;

    return metricas;
}

static t_metricas *buscar_por_pid(u_int32_t pid)
{
    char *_pid = string_itoa(pid);
    t_metricas *metricas = (t_metricas *)dictionary_get(metricas_procesos, _pid);
    free(_pid);

    return metricas;
}
