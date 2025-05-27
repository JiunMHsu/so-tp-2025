#include "estado.h"

q_estado *crear_estado()
{
    q_estado *estado = malloc(sizeof(q_estado));

    estado->lista = mlist_create();
    estado->hay_proceso = malloc(sizeof(sem_t));
    sem_init(estado->hay_proceso, 0, 0);

    return estado;
}

void push_proceso(q_estado *estado, t_pcb *pcb)
{
    mlist_push_as_queue(estado->lista, pcb);
    sem_post(estado->hay_proceso);
}

// TODO: Revisar tipado
void ordered_insert_proceso(q_estado *estado, t_pcb *pcb, int (*comparador)(t_pcb *, t_pcb *))
{
    mlist_add_sorted(estado->lista, pcb, comparador);
    sem_post(estado->hay_proceso);
}

t_pcb *pop_proceso(q_estado *estado)
{
    sem_wait(estado->hay_proceso);
    return (t_pcb *)mlist_pop_as_queue(estado->lista);
}

t_pcb *peek_proceso(q_estado *estado)
{
    t_pcb *peeked = (t_pcb *)mlist_peek(estado->lista);
    sem_post(estado->hay_proceso);
    return peeked;
}

t_pcb *remove_proceso(q_estado *estado, u_int32_t pid)
{
    int32_t _es_proceso(void *pcb)
    {
        t_pcb *proceso = (t_pcb *)pcb;
        return proceso->pid == pid;
    };

    sem_wait(estado->hay_proceso);
    return (t_pcb *)mlist_remove_by_condition(estado->lista, &_es_proceso);
}

t_list *get_pids(q_estado *estado)
{
    t_list *pids = list_create();
    void _agregar_pid_a_lista(void *ptr_pcb)
    {
        t_pcb *pcb = (t_pcb *)ptr_pcb;
        u_int32_t *pid = malloc(sizeof(u_int32_t));
        *pid = pcb->pid;
        list_add(pids, pid);
    };

    mlist_iterate(estado->lista, &_agregar_pid_a_lista);
    return pids;
}

int8_t hay_proceso(q_estado *estado)
{
    return !mlist_is_empty(estado->lista);
}

void destruir_estado(q_estado *estado)
{
    if (estado == NULL)
        return;

    void _destruir_pcb(void *pcb)
    {
        t_pcb *proceso = (t_pcb *)pcb;
        destruir_pcb(proceso);
    };

    mlist_clean(estado->lista, &_destruir_pcb);
    mlist_destroy(estado->lista);
    sem_destroy(estado->hay_proceso);
    free(estado->hay_proceso);
    free(estado);

    estado = NULL;
}
