#include "plani_mediano_plazo.h"

static q_estado *q_blocked;
static q_estado *q_susp_blocked;
static q_estado *q_susp_ready;

static algoritmo_planificacion algoritmo;

static u_int64_t tiempo_espera;

static pthread_mutex_t timer_pool_mutex;
static t_list *timer_pool;

static void suspender_proceso(t_pcb *proceso);

static void manejar_fallo(t_pcb *proceso);
static void insertar_en_suspended_ready(t_pcb *proceso);

static t_cronometro *crear_cronometro();
static bool _cronometro_libre(void *_cronometro);
static void *cronometrar(void *_cronometro);

void inicializar_planificador_mediano_plazo()
{
    q_blocked = crear_estado(BLOCKED);
    q_susp_blocked = crear_estado(SUSPENDED_BLOCKED);
    q_susp_ready = crear_estado(SUSPENDED_READY);

    algoritmo = get_alg_ingreso_a_ready();

    tiempo_espera = get_tiempo_suspension();

    pthread_mutex_init(&timer_pool_mutex, NULL);
    timer_pool = list_create();
}

static bool _cronometro_libre(void *_cronometro)
{
    t_cronometro *cronometro = (t_cronometro *)_cronometro;
    return cronometro->esta_libre == 1;
}

void insertar_en_blocked(t_pcb *proceso)
{
    push_proceso(q_blocked, proceso);
    t_cronometro *cronometro = NULL;

    pthread_mutex_lock(&timer_pool_mutex);
    cronometro = (t_cronometro *)list_find(timer_pool, &_cronometro_libre);

    if (cronometro == NULL)
    {
        cronometro = crear_cronometro();
        pthread_create(&(cronometro->rutina_consumo), NULL, &cronometrar, cronometro);
        pthread_detach(cronometro->rutina_consumo);

        list_add(timer_pool, cronometro);
    }

    cronometro->proceso = proceso;
    cronometro->esta_libre = 0;
    sem_post(cronometro->hay_proceso);
    pthread_mutex_unlock(&timer_pool_mutex);
}

void desbloquear_proceso(t_pcb *proceso, u_int8_t fallo)
{
    if (fallo)
        return manejar_fallo(proceso);

    switch (get_estado_pcb(proceso))
    {
    case BLOCKED:
        insertar_en_ready(proceso);
        remove_proceso(q_blocked, proceso->pid);
        return;
    case SUSPENDED_BLOCKED:
        insertar_en_suspended_ready(proceso);
        remove_proceso(q_susp_blocked, proceso->pid);
        return;
    default:
        log_mensaje_error("proceso no bloqueado, ni bloqueado suspendido.");
        return;
    }
}

static void manejar_fallo(t_pcb *proceso)
{
    t_state estado = get_estado_pcb(proceso);
    insertar_en_exit(proceso);

    switch (estado)
    {
    case BLOCKED:
        remove_proceso(q_blocked, proceso->pid);
        break;
    case SUSPENDED_BLOCKED:
        remove_proceso(q_susp_blocked, proceso->pid);
        break;
    default:
        log_mensaje_error("proceso no bloqueado, ni bloqueado suspendido.");
        return;
    }
}

static void insertar_en_suspended_ready(t_pcb *proceso)
{
    switch (algoritmo)
    {
    case FIFO:
        push_proceso(q_susp_ready, proceso);
        break;
    case PMCP:
        ordered_insert_proceso(q_susp_ready, proceso, &es_de_menor_tamanio_que);
        break;
    default: // caso SJF, SRT, no debería ocurrir nunca
        log_mensaje_error("Algoritmo de desuspención no soportado.");
        return;
    }

    puede_admitir_proceso_nuevo();
}

t_pcb *desuspender_proceso_ready()
{
    t_pcb *proceso = peek_proceso(q_susp_ready);
    int32_t respuesta = solicitar_swap_in(proceso->pid);

    if (!respuesta)
    {
        log_mensaje_error("Error al solicitar swap in del proceso suspendido.");
        return NULL; // No se pudo desuspender
    }

    return remove_proceso(q_susp_ready, proceso->pid);
}

int8_t hay_proceso_susp_ready()
{
    return hay_proceso(q_susp_ready);
}

static void suspender_proceso(t_pcb *proceso)
{
    if (get_estado_pcb(proceso) != BLOCKED)
        return;

    push_proceso(q_susp_blocked, proceso);
    remove_proceso(q_blocked, proceso->pid);
    int32_t resultado = solicitar_swap_out(proceso->pid);
    if (!resultado)
    {
        log_mensaje_error("Error al solicitar swap out del proceso.");
        exit(1);
    }

    puede_admitir_proceso_nuevo();
}

static t_cronometro *crear_cronometro()
{
    t_cronometro *cronometro = malloc(sizeof(t_cronometro));
    cronometro->hay_proceso = malloc(sizeof(sem_t));
    sem_init(cronometro->hay_proceso, 0, 0);
    cronometro->proceso = NULL;
    cronometro->tiempo = tiempo_espera;
    cronometro->esta_libre = 1;
    cronometro->rutina_consumo = 0;

    return cronometro;
}

static void *cronometrar(void *_cronometro)
{
    t_cronometro *cronometro = (t_cronometro *)_cronometro;

    while (1)
    {
        sem_wait(cronometro->hay_proceso);
        log_arranca_cronometro(cronometro->proceso->pid, cronometro->tiempo);
        usleep(cronometro->tiempo * 1000); // Convertir a microsegundos

        suspender_proceso(cronometro->proceso);
        cronometro->proceso = NULL;
        cronometro->esta_libre = 1;
    }

    return NULL;
}
