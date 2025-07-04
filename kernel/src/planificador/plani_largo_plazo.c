#include "plani_largo_plazo.h"

static q_estado *q_new;
static q_estado *q_exit;

static u_int32_t pid_count;

static algoritmo_planificacion algoritmo;

static sem_t *puede_crearse_proceso;

static void *admitir_proceso(void *_);
static void *finalizar_proceso(void *_);

void inicializar_planificador_largo_plazo(q_estado *q_new, q_estado *q_exit)
{
    puede_crearse_proceso = malloc(sizeof(sem_t));
    sem_init(puede_crearse_proceso, 0, 1);

    pid_count = 0;

    algoritmo = get_alg_ingreso_a_ready();
    q_new = q_new;
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
        ordered_insert_proceso(q_new, pcb, &es_de_menor_tamanio_que);
        break;
    default: // caso SJF, SRT, no debería ocurrir nunca
        log_mensaje_error("Algoritmo de ingreso a NEW no soportado.");
        return;
    }

    log_creacion_proceso(pcb->pid);
    puede_admitir_proceso_nuevo();
}

void puede_admitir_proceso_nuevo()
{
    sem_post(puede_crearse_proceso);
}

static void *admitir_proceso(void *_)
{
    while (1)
    {
        sem_wait(puede_crearse_proceso);
        t_pcb *pcb = NULL;

        if (hay_proceso_susp_ready())
        {
            pcb = desuspender_proceso_ready();
            if (pcb == NULL) // si no se pudo desuspender
                continue;

            insertar_en_ready(pcb);
            sem_post(puede_crearse_proceso);
            continue;
        }

        pcb = peek_proceso(q_new);
        int32_t solicitud = solicitar_creacion_proceso(pcb->pid, pcb->tamanio, pcb->ejecutable);

        // caso 0 o -1 (ver si hacer alguna direfencia y que el sistema paniquee)
        if (solicitud < 1)
            continue;

        pcb = remove_proceso(q_new, pcb->pid);
        insertar_en_ready(pcb);
        sem_post(puede_crearse_proceso);
    }

    return NULL;
}

void insertar_en_exit(t_pcb *proceso)
{
    push_proceso(q_exit, proceso);
}

static void *finalizar_proceso(void *_)
{
    while (1)
    {
        t_pcb *proceso = peek_proceso(q_exit);
        int32_t res_solicitud = solicitar_finalizacion_proceso(proceso->pid);

        // capaz es medio al pedo la validación,
        // porque tampoco se define lo que debería suceder
        // si la memoria no pudo finalizar el proceso.
        if (res_solicitud == 1)
        {
            puede_admitir_proceso_nuevo();
            proceso = remove_proceso(q_exit, proceso->pid);

            log_metricas_proceso(proceso->pid,
                                 proceso->metricas_estado,
                                 proceso->metricas_tiempo);
            log_finalizacion_proceso(proceso->pid);
            destruir_pcb(proceso);
        }
    }

    return NULL;
}
