#include "plani_corto_plazo.h"

static q_estado *q_ready;
static q_estado *q_executing;

static sem_t *hay_cpu_libre;      // solo usado en SJF
static sem_t *puede_replanificar; // solo usado en SRT

static double alpha;
static double estimacion_inicial;

static algoritmo_planificacion algoritmo_en_uso;

static double estimar_rafaga(double anterior_estimado, double real_anterior);

static int64_t get_rafaga_restante_estimado(t_pcb *pcb);
static t_pcb *_es_de_menor_rafaga(t_pcb *proceso_a, t_pcb *proceso_b);
static t_pcb *_mayor_rafaga_restante(t_pcb *a, t_pcb *b);

static void *planificar_por_fifo(void *_);
static void *planificar_por_sjf(void *_);
static void *planificar_por_srt(void *_);

static void *manejar_desalojado(void *_);

void inicializar_planificador_corto_plazo(q_estado *q_ready, q_estado *q_executing)
{
    q_ready = q_ready;
    q_executing = q_executing;

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
    u_int64_t rafaga_estimacion = estimar_rafaga(get_ultima_estimacion_rafaga_pcb(proceso),
                                                 get_ultima_rafaga_pcb(proceso));
    set_estimacion_rafaga_pcb(proceso, rafaga_estimacion);

    push_proceso(q_ready, proceso);

    if (algoritmo_en_uso == SRT)
        sem_wait(puede_replanificar);
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
        t_pcb *proceso = pop_proceso(q_ready);

        ejecutar(proceso);
        push_proceso(q_executing, proceso);
    }

    return NULL;
}

static void *planificar_por_sjf(void *_)
{
    hay_cpu_libre = malloc(sizeof(sem_t));
    sem_init(hay_cpu_libre, 0, 1);

    while (1)
    {
        sem_wait(hay_cpu_libre);
        t_pcb *proceso = pop_proceso_minimo(q_ready, &_es_de_menor_rafaga);

        ejecutar(proceso); // bloquante si no hay CPU libre
        push_proceso(q_executing, proceso);
    }

    return NULL;
}

static int64_t get_rafaga_restante_estimado(t_pcb *pcb)
{
    int64_t tiempo_ejecutando = get_tiempo_estado_actual_pcb(pcb);
    return get_estimacion_rafaga_pcb(pcb) - tiempo_ejecutando;
}

static t_pcb *_mayor_rafaga_restante(t_pcb *proceso_a, t_pcb *proceso_b)
{
    if (get_rafaga_restante_estimado(proceso_a) >= get_rafaga_restante_estimado(proceso_b))
        return proceso_a;

    return proceso_b;
}

static void *planificar_por_srt(void *_)
{
    puede_replanificar = malloc(sizeof(sem_t));
    sem_init(puede_replanificar, 0, 1);

    while (1)
    {
        sem_wait(puede_replanificar); // TODO: ver bien si no hay bug
        t_pcb *proceso = peek_proceso_minimo(q_ready, &_es_de_menor_rafaga);

        if (hay_cpu())
        {
            proceso = remove_proceso(q_ready, proceso->pid);
            ejecutar(proceso); // bloquante si no hay CPU libre
            push_proceso(q_executing, proceso);
            continue;
        }

        t_pcb *proceso_mayor_rafaga_restante = peek_proceso_maximo(q_executing, &_mayor_rafaga_restante);

        // dado que el algoritmo compara las ráfagas "a ejecutar",
        // se hace la comparación del restante estimado.
        if (get_estimacion_rafaga_pcb(proceso) >= get_rafaga_restante_estimado(proceso_mayor_rafaga_restante))
            continue;

        enviar_interrupcion(proceso_mayor_rafaga_restante->pid);
        proceso = remove_proceso(q_ready, proceso->pid);
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

        if (algoritmo_en_uso == SJF)
            sem_post(hay_cpu_libre);

        if (algoritmo_en_uso == SRT)
            sem_post(puede_replanificar);

        t_pcb *proceso = remove_proceso(q_executing, desalojado->pid);

        set_program_counter_pcb(proceso, desalojado->program_counter);
        set_ultima_estimacion_rafaga_pcb(proceso, get_estimacion_rafaga_pcb(proceso));
        set_ultima_rafaga_pcb(proceso, get_tiempo_estado_actual_pcb(proceso));

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