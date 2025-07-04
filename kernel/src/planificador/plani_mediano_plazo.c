#include "plani_mediano_plazo.h"

static q_estado *q_susp_ready;
static q_estado *q_susp_blocked;
static q_estado *q_blocked;

static algoritmo_planificacion algoritmo;

static u_int64_t tiempo_espera;

static pthread_mutex_t timer_pool_mutex;
static t_list *timer_pool;

static void suspender_proceso(t_pcb *proceso);

static void manejar_fallo(t_pcb *proceso);
static void insertar_en_suspended_ready(t_pcb *proceso);

static t_cronometro *crear_cronometro(u_int64_t tiempo_espera);
static bool _cronometro_libre(void *_cronometro);
static void *cronometrar(void *_cronometro);

void inicializar_planificador_mediano_plazo(q_estado *q_susp_ready,
                                            q_estado *q_susp_blocked,
                                            q_estado *q_blocked)
{
    q_susp_ready = q_susp_ready;
    q_susp_blocked = q_susp_blocked;
    q_blocked = q_blocked;

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
        cronometro = crear_cronometro(tiempo_espera);
        pthread_create(&(cronometro->rutina_consumo), NULL, &cronometrar, cronometro);
        pthread_detach(cronometro->rutina_consumo);

        list_add(timer_pool, cronometro);
    }

    cronometro->pid = proceso->pid;
    cronometro->esta_libre = 0;
    sem_post(cronometro->hay_proceso);
    pthread_mutex_unlock(&timer_pool_mutex);
}

void desbloquear_proceso(t_pcb *proceso, int8_t fallo)
{
    if (fallo)
        return manejar_fallo(proceso);

    t_pcb *pcb = NULL;
    switch (get_estado_pcb(proceso))
    {
    case BLOCKED:
        pcb = remove_proceso(q_blocked, proceso->pid);
        insertar_en_ready(pcb);
        return;
    case SUSPENDED_BLOCKED:
        pcb = remove_proceso(q_susp_blocked, proceso->pid);
        insertar_en_suspended_ready(pcb);
        return;
    default:
        log_mensaje_error("proceso no bloqueado, ni bloqueado suspendido.");
        return;
    }
}

static void manejar_fallo(t_pcb *proceso)
{
    t_pcb *pcb = NULL;
    switch (get_estado_pcb(proceso))
    {
    case BLOCKED:
        pcb = remove_proceso(q_blocked, proceso->pid);
        break;
    case SUSPENDED_BLOCKED:
        pcb = remove_proceso(q_susp_blocked, proceso->pid);
        break;
    default:
        log_mensaje_error("proceso no bloqueado, ni bloqueado suspendido.");
        return;
    }

    insertar_en_exit(pcb);
}

static void insertar_en_suspended_ready(t_pcb *proceso)
{
    switch (algoritmo)
    {
    case FIFO:
        push_proceso(q_susp_ready, pcb);
        return;
    case PMCP:
        ordered_insert_proceso(q_susp_ready, pcb, &es_de_menor_tamanio_que);
        return;
    default: // caso SJF, SRT, no debería ocurrir nunca
        log_mensaje_error("Algoritmo de desuspención no soportado.");
        return;
    }
}

// TODO: Implementar la logica de desuspender un proceso de susp_ready
t_pcb *desuspender_proceso_ready() {}

int8_t hay_proceso_susp_ready()
{
    return hay_proceso(q_susp_ready);
}

static void suspender_proceso(t_pcb *proceso)
{
    push_proceso(q_susp_blocked, proceso);
    solicitar_swap_out(proceso->pid); // en teoria esto devuelve algo, pero no se si hace falta evaluarlo
    puede_admitir_proceso_nuevo();
}

static t_cronometro *crear_cronometro(u_int64_t tiempo_espera)
{
    t_cronometro *cronometro = malloc(sizeof(t_cronometro));
    cronometro->hay_proceso = malloc(sizeof(sem_t));
    sem_init(cronometro->hay_proceso, 0, 0);
    cronometro->pid = 0;
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
        usleep(cronometro->tiempo * 1000); // Convertir a microsegundos

        t_pcb *proceso = remove_proceso(q_blocked, cronometro->pid);
        if (proceso != NULL)
            suspender_proceso(proceso);

        cronometro->esta_libre = 1;
    }

    return NULL;
}
