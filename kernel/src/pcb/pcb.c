#include "pcb.h"

t_pcb *crear_pcb(u_int32_t pid, u_int32_t tamanio, char *ejecutable)
{
    t_pcb *pcb = malloc(sizeof(t_pcb));

    pcb->pid = pid;
    pcb->program_counter = 0;
    pcb->tamanio = tamanio;
    pcb->ejecutable = strdup(ejecutable);
    pcb->estado = -1;
    pcb->metricas_estado = dictionary_create();
    pcb->metricas_tiempo = dictionary_create();

    return pcb;
}

void destruir_pcb(t_pcb *pcb)
{
    free(pcb->ejecutable);
    dictionary_destroy_and_destroy_elements(pcb->metricas_estado, free);

    // si metricas de tiempo es una lista, recorrer y liberar
    dictionary_destroy_and_destroy_elements(pcb->metricas_tiempo, free);
    free(pcb);
}

void set_estado_pcb(t_pcb *pcb, t_state estado)
{
    // se podría aprovechar para actualizar
    // las métricas de estado y tiempo
    pcb->estado = estado;
}

void set_program_counter_pcb(t_pcb *pcb, u_int32_t program_counter)
{
    pcb->program_counter = program_counter;
}
