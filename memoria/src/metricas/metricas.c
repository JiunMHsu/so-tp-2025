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
    pthread_mutex_lock(&mutex_metricas);
    char *_pid = string_itoa(pid);

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

t_metricas *remover_metricas_para(u_int32_t pid) {}

void destruir_metricas(t_metricas *metricas) {}

void incrementar_acceso_tabla(u_int32_t pid) {}
void incrementar_instruccion_solicitada(u_int32_t pid) {}
void incrementar_swap_out(u_int32_t pid) {}
void incrementar_swap_in(u_int32_t pid) {}
void incrementar_lectura(u_int32_t pid) {}
void incrementar_escritura(u_int32_t pid) {}

static t_metricas *crear_metricas() {}

static t_metricas *buscar_por_pid(u_int32_t pid) {}
