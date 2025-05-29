#include "plani_largo_plazo.h"

static q_estado *q_new;
static q_estado *q_ready;
static q_estado *q_exit;

static u_int16_t pid_count;

static algoritmo_planificacion algoritmo;

static sem_t *puede_crearse_proceso;

static void *admitir_proceso(void *_);
static int _es_de_menor_tamanio_que(t_pcb *proceso_a, t_pcb *proceso_b);
static void *finalizar_proceso(void *_);

void inicializar_planificador_largo_plazo(q_estado *q_new,
                                          q_estado *q_ready,
                                          q_estado *q_exit)
{
    puede_crearse_proceso = malloc(sizeof(sem_t));
    sem_init(puede_crearse_proceso, 0, 1);

    pid_count = 0;

    algoritmo = get_alg_ingreso_a_ready();
    q_new = q_new;
    q_ready = q_ready;
    q_exit = q_exit;

    pthread_t rutinas[2];

    pthread_create(&rutinas[0], NULL, &admitir_proceso, NULL);
    pthread_detach(rutinas[0]);

    pthread_create(&rutinas[1], NULL, &finalizar_proceso, NULL);
    pthread_detach(rutinas[1]);
}

void insertar_proceso_nuevo(char *pseudocodigo, u_int32_t tamanio_proceso)
{
    t_pcb *pcb = crear_pcb(pid_count++,
                           tamanio_proceso,
                           pseudocodigo,
                           get_estimacion_inicial());

    switch (algoritmo)
    {
    case FIFO:
        push_proceso(q_new, pcb);
        break;
    case PMCP:
        // TODO: Definir el criterio de ordenamiento
        ordered_insert_proceso(q_new, pcb, &_es_de_menor_tamanio_que);
        break;
    default: // caso SJF, SRT, no debería ocurrir nunca
        log_mensaje_error("Algoritmo de ingreso a NEW no soportado.");
        return;
    }

    log_creacion_proceso(pcb->pid);
}

static int32_t _es_de_menor_tamanio_que(t_pcb *proceso_a, t_pcb *proceso_b)
{
    return proceso_a->tamanio <= proceso_b->tamanio;
}

static void *admitir_proceso(void *_)
{
    while (1)
    {
        sem_wait(puede_crearse_proceso);
        t_pcb *pcb = peek_proceso(q_new);
        int32_t solicitud = solicitar_creacion_proceso(pcb->pid, pcb->tamanio, pcb->ejecutable);

        if (solicitud < 1) // caso 0 o -1 (ver si hacer alguna direfencia y que el sistema paniquee)
            continue;

        push_proceso(q_ready, pcb);
        remove_proceso(q_new, pcb->pid); // porque peek no remueve de la lista
        sem_post(puede_crearse_proceso);
    }

    return NULL;
}

// TODO: Implementar inserción en EXIT
void insertar_en_exit(t_pcb *proceso) {}

static void *finalizar_proceso(void *_)
{
    while (1)
    {
        t_pcb *proceso = peek_proceso(q_exit);
        int32_t res_solicitud = solicitar_finalizacion_proceso(proceso->pid);

        if (res_solicitud == 1)
        {
            sem_post(puede_crearse_proceso);
            proceso = remove_proceso(q_exit, proceso->pid);

            // TODO: Loggear métricas del proceso
            log_finalizacion_proceso(proceso->pid);
            destruir_pcb(proceso);
        }
    }

    return NULL;
}
