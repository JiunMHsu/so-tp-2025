#include "pcb.h"

static void actualizar_metricas_estado(t_pcb *pcb);
static void actualizar_metricas_tiempo(t_pcb *pcb);

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

t_pcb *crear_pcb(u_int32_t pid, u_int32_t tamanio, char *ejecutable, u_int64_t est_rafaga_inicial)
{
    t_pcb *pcb = malloc(sizeof(t_pcb));

    pcb->pid = pid;
    pcb->program_counter = 0;
    pcb->tamanio = tamanio;
    pcb->ejecutable = strdup(ejecutable);
    pcb->estado = -1;

    pcb->metricas_estado = dictionary_create();
    pcb->metricas_tiempo = dictionary_create();
    dictionary_put(pcb->metricas_estado, strdup(get_estado_string(NEW)), list_create());
    dictionary_put(pcb->metricas_estado, strdup(get_estado_string(READY)), list_create());
    dictionary_put(pcb->metricas_estado, strdup(get_estado_string(EXEC)), list_create());
    dictionary_put(pcb->metricas_estado, strdup(get_estado_string(BLOCKED)), list_create());
    dictionary_put(pcb->metricas_estado, strdup(get_estado_string(SUSPENDED_BLOCKED)), list_create());
    dictionary_put(pcb->metricas_estado, strdup(get_estado_string(SUSPENDED_READY)), list_create());
    dictionary_put(pcb->metricas_estado, strdup(get_estado_string(EXIT)), list_create());

    pcb->temporal = NULL;

    pcb->ultima_estimacion_rafaga = est_rafaga_inicial;
    pcb->estimacion_rafaga = -1;

    pthread_mutex_init(&pcb->mutex, NULL);

    return pcb;
}

// TODO: refactor, agregar destructores para los campos que lo requieran
void destruir_pcb(t_pcb *pcb)
{
    free(pcb->ejecutable);
    dictionary_destroy_and_destroy_elements(pcb->metricas_estado, free);

    // si metricas de tiempo es una lista, recorrer y liberar
    dictionary_destroy_and_destroy_elements(pcb->metricas_tiempo, free);
    free(pcb);
}

t_state get_estado_pcb(t_pcb *pcb)
{
    pthread_mutex_lock(&pcb->mutex);
    t_state estado = pcb->estado;
    pthread_mutex_unlock(&pcb->mutex);
    return estado;
}

u_int64_t get_ultima_estimacion_rafaga_pcb(t_pcb *pcb)
{
    pthread_mutex_lock(&pcb->mutex);
    u_int64_t ultima_estimacion = pcb->ultima_estimacion_rafaga;
    pthread_mutex_unlock(&pcb->mutex);
    return ultima_estimacion;
}

u_int64_t get_estimacion_rafaga_pcb(t_pcb *pcb)
{
    pthread_mutex_lock(&pcb->mutex);
    u_int64_t estimacion = pcb->estimacion_rafaga;
    pthread_mutex_unlock(&pcb->mutex);
    return estimacion;
}

void set_program_counter_pcb(t_pcb *pcb, u_int32_t program_counter)
{
    pthread_mutex_lock(&pcb->mutex);
    pcb->program_counter = program_counter;
    pthread_mutex_unlock(&pcb->mutex);
}

void set_estado_pcb(t_pcb *pcb, t_state estado)
{
    pthread_mutex_lock(&pcb->mutex);

    pcb->estado = estado;
    actualizar_metricas_estado(pcb);
    actualizar_metricas_tiempo(pcb);

    pthread_mutex_unlock(&pcb->mutex);
}

static void actualizar_metricas_estado(t_pcb *pcb)
{
    char *estado_string = get_estado_string(pcb->estado);
    u_int32_t *cantidad;

    // en este caso, si key existe, implica que hay al menos 1 ocurrencia (*cantidad = 1)
    // no hace falta evaluar si el valor es NULL
    if (dictionary_has_key(pcb->metricas_estado, estado_string))
    {
        cantidad = (u_int32_t *)dictionary_get(pcb->metricas_estado, estado_string);
        (*cantidad)++;
        return;
    }

    cantidad = malloc(sizeof(u_int32_t));
    *cantidad = 1;
    dictionary_put(pcb->metricas_estado, strdup(estado_string), cantidad);
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
    t_list *lista_tiempos = (t_list *)dictionary_get(pcb->metricas_estado, estado_string);

    u_int64_t *tiempo = malloc(sizeof(u_int64_t));
    *tiempo = temporal_gettime(pcb->temporal);
    list_add(lista_tiempos, tiempo);

    temporal_destroy(pcb->temporal);
    pcb->temporal = temporal_create();
}

void set_ultima_estimacion_rafaga_pcb(t_pcb *pcb, u_int64_t ultima_estimacion)
{
    pthread_mutex_lock(&pcb->mutex);
    pcb->ultima_estimacion_rafaga = ultima_estimacion;
    pthread_mutex_unlock(&pcb->mutex);
}

void set_estimacion_rafaga_pcb(t_pcb *pcb, u_int64_t estimacion)
{
    pthread_mutex_lock(&pcb->mutex);
    pcb->estimacion_rafaga = estimacion;
    pthread_mutex_unlock(&pcb->mutex);
}
