#include "plani_corto_plazo.h"

static algoritmo_planificacion algoritmo;
static q_estado *ready;
static double alpha;
static double estimacion_inicial;


void inicializar_planificador_corto_plazo(algoritmo_planificacion algoritmo_corto_plazo, q_estado *estado_ready)
{
    algoritmo = algoritmo_corto_plazo;
    ready = estado_ready;
    alpha = get_alfa_estimacion();
    // estimacion_inicial = get_estimacion_inicial();

}

t_pcb *seleccionar_proximo_proceso() 
{
    pthread_mutex_lock(&mutex_ready);

    t_pcb *proximo = NULL;

    switch (algoritmo) {
        case FIFO:
            proximo = pop_proceso(ready);
            break;
        case SJF:
            proximo = pop_sjf(ready); //TODO
            break;
        case SRT:
            proximo = pop_srt(ready); //TODO
            break;
    }

    pthread_mutex_unlock(&mutex_ready);
    return proximo;
}

double estimar_rafaga(double anterior_estimado, double real_anterior) 
{
    return alpha * real_anterior + (1 - alpha) * anterior_estimado;
}