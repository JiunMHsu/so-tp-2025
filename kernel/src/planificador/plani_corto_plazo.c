#include "plani_corto_plazo.h"

static q_estado *q_ready;
static q_estado *q_executing;

static sem_t *hay_cpu_libre;
static sem_t *hay_proceso_ready;

static double alpha;
static double estimacion_inicial;

static algoritmo_planificacion algoritmo_en_uso;

static double estimar_rafaga(double anterior_estimado, double real_anterior);
static t_pcb *_es_de_menor_rafaga(t_pcb *a, t_pcb *b);

static void *planificar_por_fifo(void *_);
static void *planificar_por_sjf(void *_);
static void *planificar_por_srt(void *_);

static void *manejar_desalojado(void *_);

void inicializar_planificador_corto_plazo(q_estado *q_ready, q_estado *q_executing)
{
    q_ready = q_ready;
    q_executing = q_executing;

    hay_cpu_libre = malloc(sizeof(sem_t));
    sem_init(hay_cpu_libre, 0, 1);

    hay_proceso_ready = malloc(sizeof(sem_t));
    sem_init(hay_proceso_ready, 0, 0);

    alpha = get_alfa_estimacion();
    estimacion_inicial = get_estimacion_inicial();

    void *(*planificador_corto_plazo)(void *) = NULL;

    switch (get_alg_plani_corto_plazo())
    {
    case FIFO:
        planificador_corto_plazo = &planificar_por_fifo;
        algoritmo_en_uso = FIFO;
        break;
    case SJF:
        planificador_corto_plazo = &planificar_por_sjf;
        algoritmo_en_uso = SJF;
        break;
    case SRT:
        planificador_corto_plazo = &planificar_por_srt;
        algoritmo_en_uso = SRT;
        break;
    default:
        log_mensaje_error("Algoritmo de planificación de corto plazo no soportado.");
        return;
    }

    pthread_t rutinas[2];

    pthread_create(&rutinas[0], NULL, planificador_corto_plazo, NULL);
    pthread_detach(rutinas[0]);

    pthread_create(&rutinas[1], NULL, &manejar_desalojado, NULL);
    pthread_detach(rutinas[1]);
}

void insertar_en_ready(t_pcb *proceso)
{
    u_int64_t rafaga_estimacion = estimar_rafaga(proceso->ultima_estimacion_rafaga, proceso->ultima_rafaga);
    set_estimacion_rafaga_pcb(proceso, rafaga_estimacion);

    push_proceso(q_ready, proceso);
    sem_post(hay_proceso_ready);
}

static double estimar_rafaga(double anterior_estimado, double real_anterior)
{
    return alpha * real_anterior + (1 - alpha) * anterior_estimado;
}

static t_pcb *_es_de_menor_rafaga(t_pcb *proceso_a, t_pcb *proceso_b)
{
    if (get_estimacion_rafaga_pcb(proceso_a) < get_estimacion_rafaga_pcb(proceso_b))
        return proceso_a;

    return proceso_b;
}

static void *planificar_por_fifo(void *_)
{
    while (1)
    {
        sem_wait(hay_cpu_libre);
        sem_wait(hay_proceso_ready); // capaz es medio al pedo
        t_pcb *proceso = pop_proceso(q_ready);

        ejecutar(proceso);
        push_proceso(q_executing, proceso);
    }

    return NULL;
}

static void *planificar_por_sjf(void *_)
{
    while (1)
    {
        sem_wait(hay_cpu_libre);
        sem_wait(hay_proceso_ready); // capaz es medio al pedo
        t_pcb *proceso = pop_proceso_minimo(q_ready, &_es_de_menor_rafaga);

        ejecutar(proceso); // bloquante si no hay CPU libre
        push_proceso(q_executing, proceso);
    }

    return NULL;
}

static void *planificar_por_srt(void *_)
{
    // TODO: Iniciar rutina de envío de interrupción

    while (1)
    {
        t_pcb *proceso = pop_proceso_minimo(q_ready, &_es_de_menor_rafaga);
        ejecutar(proceso); // bloquante si no hay CPU libre
        push_proceso(q_executing, proceso);
    }

    return NULL;
}

static void *manejar_desalojado(void *_)
{
    while (1)
    {
        t_desalojo *desalojado = get_desalojado(); // bloquante si no hay finalizados
        sem_post(hay_cpu_libre);

        t_pcb *proceso = remove_proceso(q_executing, desalojado->pid);
        proceso->program_counter = desalojado->program_counter;
        // TODO: Actualizar métricas de tiempo

        switch (desalojado->motivo)
        {
        case SCHEDULER_INT:
            insertar_en_ready(proceso);
            break;
        case SYSCALL:
            manejar_syscall(proceso, desalojado->syscall);
            break;
        default: // No debería ocurrir nunca
            log_mensaje_error("Motivo de desalojo no soportado.");
            break;
        }

        destruir_desalojo(desalojado);
    }

    return NULL;
}