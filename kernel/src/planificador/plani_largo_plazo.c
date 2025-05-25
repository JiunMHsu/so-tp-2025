#include "plani_largo_plazo.h"

q_estado *new;
q_estado *ready;
q_estado *exit_; // con guion bajo porque si no se confundia con una funcion de stdlib

u_int16_t pid_count;

algoritmo_planificacion algoritmo;

sem_t *hay_proceso_new;
sem_t puede_crearse_proceso;
// void esperar_solicitud(Proceso *proceso_nuevo);

static void *admitir_proceso(void *_);
static void crear_proceso(t_pcb *pcb);

void inicializar_planificador_largo_plazo(algoritmo_planificacion alg_planificacion,
                                          q_estado *estado_new,
                                          q_estado *estado_ready,
                                          q_estado *estado_exit)
{
    hay_proceso_new = malloc(sizeof(sem_t));
    sem_init(hay_proceso_new, 0, 0);
    sem_init(&puede_crearse_proceso, 0, 0);

    pid_count = 0;

    algoritmo = alg_planificacion;
    new = estado_new;
    ready = estado_ready;
    exit_ = estado_exit;

    pthread_t hilo_admision;
    pthread_create(&hilo_admision, NULL, admitir_proceso, NULL);
}

void pusheo_a_new_por_algoritmo(t_pcb *pcb)
{
    switch (algoritmo)
    {
    case FIFO:
        push_proceso(new, pcb);
        break;
    case PMCP:
        // TODO: Definir el criterio de ordenamiento
        ordered_insert_proceso(new, pcb, &criterio_prioridad);
        break;
    }
}

void insertar_proceso_nuevo(char *pseudocodigo, u_int32_t tamanio_proceso)
{
    t_pcb *pcb = crear_pcb(pid_count++, tamanio_proceso, pseudocodigo);
    pusheo_a_new_por_algoritmo(pcb);
    // sem_post(hay_proceso_new);
}

static void *admitir_proceso(void *_)
{
    while (1)
    {
        sem_wait(hay_proceso_new);

        t_pcb *pcb = pop_proceso(new);

        int32_t solicitud = solicitar_creacion_proceso(pcb->pid, pcb->tamanio, pcb->pseudocodigo);

        if (solicitud)
        {
            push_proceso(ready, pcb);
        }
        else
        {
            pusheo_a_new_por_algoritmo(pcb);
            sem_wait(&puede_crearse_proceso);
            sem_post(hay_proceso_new);
        }
    }

    return NULL;
}

void rutina_exit(t_pcb *proceso_finalizado)
{

    int32_t solicitud = solicitar_finalizacion_proceso(proceso_finalizado);

    if (solicitud)
    {
        sem_post(&puede_crearse_proceso);
        push_proceso(exit_, proceso_finalizado);
    }
}
