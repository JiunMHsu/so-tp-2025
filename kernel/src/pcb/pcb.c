#include "pcb.h"

static void actualizar_metricas_estado(t_pcb *pcb);
static void actualizar_metricas_tiempo(t_pcb *pcb);

static void _destruir_lista_tiempos(void *elemento);

char *get_estado_string(t_state estado)
{
    switch (estado)
    {
    case NEW:
        return "NEW";
    case READY:
        return "READY";
    case EXEC:
        return "EXEC";
    case BLOCKED:
        return "BLOCKED";
    case SUSPENDED_BLOCKED:
        return "SUSPENDED_BLOCKED";
    case SUSPENDED_READY:
        return "SUSPENDED_READY";
    case EXIT:
        return "EXIT";
    default:
        return "UNKNOWN";
    }
}

t_pcb *crear_pcb(u_int32_t pid, u_int32_t tamanio, char *ejecutable, double est_rafaga_inicial)
{
    t_pcb *pcb = malloc(sizeof(t_pcb));

    pcb->pid = pid;
    pcb->program_counter = 0;
    pcb->tamanio = tamanio;
    pcb->ejecutable = strdup(ejecutable);
    pcb->estado = -1;

    pcb->metricas_estado = dictionary_create();
    pcb->metricas_tiempo = dictionary_create();

    t_state estados[7] = {NEW, READY, EXEC, BLOCKED, SUSPENDED_BLOCKED, SUSPENDED_READY, EXIT};
    for (int i = 0; i < 7; i++)
    {
        char *estado_string = get_estado_string(estados[i]);
        dictionary_put(pcb->metricas_estado, strdup(estado_string), calloc(1, sizeof(u_int32_t)));
        dictionary_put(pcb->metricas_tiempo, strdup(estado_string), list_create());
    }

    pcb->temporal = NULL;

    pcb->estimacion_rafaga = est_rafaga_inicial;
    pcb->rafaga_ejecutada = 0;

    pthread_mutex_init(&(pcb->mutex), NULL);

    return pcb;
}

void destruir_pcb(t_pcb *pcb)
{
    if (pcb == NULL)
        return;

    free(pcb->ejecutable);
    dictionary_destroy_and_destroy_elements(pcb->metricas_estado, &free);
    dictionary_destroy_and_destroy_elements(pcb->metricas_tiempo, &_destruir_lista_tiempos);

    if (pcb->temporal != NULL)
        temporal_destroy(pcb->temporal);

    pthread_mutex_destroy(&(pcb->mutex));
    free(pcb);

    pcb = NULL;
}

static void _destruir_lista_tiempos(void *elemento)
{
    t_list *lista_tiempos = (t_list *)elemento;
    list_destroy_and_destroy_elements(lista_tiempos, &free);
}

t_state get_estado_pcb(t_pcb *pcb)
{
    pthread_mutex_lock(&(pcb->mutex));
    t_state estado = pcb->estado;
    pthread_mutex_unlock(&(pcb->mutex));
    return estado;
}

u_int64_t get_tiempo_estado_actual_pcb(t_pcb *pcb)
{
    pthread_mutex_lock(&(pcb->mutex));

    if (pcb->temporal == NULL)
    {
        pthread_mutex_unlock(&(pcb->mutex));
        return 0;
    }

    temporal_stop(pcb->temporal);
    u_int64_t transcurrido = temporal_gettime(pcb->temporal);
    temporal_resume(pcb->temporal);

    pthread_mutex_unlock(&(pcb->mutex));
    return transcurrido;
}

double get_estimacion_rafaga_pcb(t_pcb *pcb)
{
    pthread_mutex_lock(&(pcb->mutex));
    double estimacion = pcb->estimacion_rafaga;
    pthread_mutex_unlock(&(pcb->mutex));
    return estimacion;
}

u_int64_t get_rafaga_ejecutada_pcb(t_pcb *pcb)
{
    pthread_mutex_lock(&(pcb->mutex));
    u_int64_t rafaga_ejecutada = pcb->rafaga_ejecutada;
    pthread_mutex_unlock(&(pcb->mutex));
    return rafaga_ejecutada;
}

void set_program_counter_pcb(t_pcb *pcb, u_int32_t program_counter)
{
    pthread_mutex_lock(&(pcb->mutex));
    pcb->program_counter = program_counter;
    pthread_mutex_unlock(&(pcb->mutex));
}

void set_estado_pcb(t_pcb *pcb, t_state estado)
{
    pthread_mutex_lock(&(pcb->mutex));

    pcb->estado = estado;
    actualizar_metricas_estado(pcb);
    actualizar_metricas_tiempo(pcb);

    pthread_mutex_unlock(&(pcb->mutex));
}

static void actualizar_metricas_estado(t_pcb *pcb)
{
    char *estado_string = get_estado_string(pcb->estado);
    u_int32_t *cantidad = (u_int32_t *)dictionary_get(pcb->metricas_estado, estado_string);
    (*cantidad)++;
}

static void actualizar_metricas_tiempo(t_pcb *pcb)
{
    if (pcb->temporal == NULL)
    {
        pcb->temporal = temporal_create();
        return;
    }

    temporal_stop(pcb->temporal);

    char *estado_string = get_estado_string(pcb->estado);
    t_list *lista_tiempos = (t_list *)dictionary_get(pcb->metricas_tiempo, estado_string);

    u_int64_t *tiempo = malloc(sizeof(u_int64_t));
    *tiempo = temporal_gettime(pcb->temporal);
    list_add(lista_tiempos, tiempo);

    temporal_destroy(pcb->temporal);
    pcb->temporal = temporal_create();
}

void set_estimacion_rafaga_pcb(t_pcb *pcb, double estimacion)
{
    pthread_mutex_lock(&(pcb->mutex));
    pcb->estimacion_rafaga = estimacion;
    pthread_mutex_unlock(&(pcb->mutex));
}

void set_rafaga_ejecutada_pcb(t_pcb *pcb, u_int64_t rafaga)
{
    pthread_mutex_lock(&(pcb->mutex));
    pcb->rafaga_ejecutada = rafaga;
    pthread_mutex_unlock(&(pcb->mutex));
}
