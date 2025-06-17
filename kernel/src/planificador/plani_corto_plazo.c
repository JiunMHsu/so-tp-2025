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
    u_int64_t rafaga_estimacion = estimar_rafaga(get_ultima_estimacion_rafaga_pcb(proceso),
                                                 get_ultima_rafaga_pcb(proceso));
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
        sem_wait(hay_proceso_ready); // capaz es medio al pedo                  creo que si, pop ya tiene implementado un semaforo
        t_pcb *proceso = pop_proceso_minimo(q_ready, &_es_de_menor_rafaga);

        ejecutar(proceso); // bloquante si no hay CPU libre
        push_proceso(q_executing, proceso);
    }

    return NULL;
}

t_pcb *proceso_mayor_rafaga()
{
    pthread_mutex_lock(&q_executing->lista->mutex);

    t_pcb *proceso_mas_largo = NULL;
    double mayor_restante = -1;

    for (int i = 0; i < list_size(q_executing->lista->elements); i++) {
        t_pcb *pcb = list_get(q_executing->lista->elements, i);

        u_int64_t tiempo_en_ejecucion = get_tiempo_estado_actual_pcb(pcb);

        double restante = pcb->estimacion_rafaga - tiempo_en_ejecucion;

        if (proceso_mas_largo == NULL || restante > mayor_restante) {
            proceso_mas_largo = pcb;
            mayor_restante = restante;
        }
    }

    pthread_mutex_unlock(&q_executing->lista->mutex);

    return proceso_mas_largo;
}


static void *planificar_por_srt(void *_)
{
    while (1)
    {
        t_pcb *proceso = pop_proceso_minimo(q_ready, &_es_de_menor_rafaga);
        t_pcb *proceso_mas_largo_ejecutando = proceso_mayor_rafaga();
        t_pcb *proceso_mas_chico = _es_de_menor_rafaga(proceso, proceso_mas_largo_ejecutando);
        
        if(proceso_mas_chico == proceso) 
        {
            pop_proceso(q_executing, proceso_mas_largo_ejecutando);
            //timer_stop(proceso_mas_largo_ejecutando);     no se si hace falta o enviar_interr ya para el tiempo
            push_proceso(q_ready, proceso_mas_largo_ejecutando);
            enviar_interrupcion(proceso_mas_largo_ejecutando->pid);
        }
        
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