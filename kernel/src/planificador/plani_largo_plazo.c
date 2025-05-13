#include "plani_largo_plazo.h"

q_estado *new;
q_estado *ready;
q_estado *exit;

u_int16_t pid_count;

static void *admitir_proceso(void *_);
static void crear_proceso(t_pcb *pcb);
// void esperar_solicitud(Proceso *proceso_nuevo);

// TODO: Implementar
void inicializar_planificador_largo_plazo(algoritmo_planificacion alg_planificacion,
                                          q_estado *estado_new,
                                          q_estado *estado_ready,
                                          q_estado *estado_exit)
{
    hay_proceso_new = malloc(sizeof(sem_t));
    sem_init(hay_proceso_new, 0, 0);

    u_int16_t pid_count = 0;

    // mas cosas
}

void insertar_a_new(char *pseudocodigo, u_int32_t tamanio_proceso)
{
    // creo el pcb y push a new

    // switch(alg_planificacion)
    // {
    //     case FIFO: (mlist_push_as_queue)
    //     case PMCP: (ordered_push)
    // }
}

// TODO: Implementar
void crear_proceso(t_pcb *pcb)
{
}

static void *admitir_proceso(void *_)
{
    while (1)
    {
        t_pcb *proceso_nuevo = pop_proceso(new);
        // crear y push a ready
        // TODO: Implementar
    }
}

// TODO: rutina exit
