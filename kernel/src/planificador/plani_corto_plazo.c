#include "plani_corto_plazo.h"

static q_estado *q_ready;
static q_estado *q_exit;

static double alpha;
static double estimacion_inicial;

static double estimar_rafaga(double anterior_estimado, double real_anterior);

static void *planificar_por_fifo(void *_);
static void *planificar_por_sjf(void *_);
static void *planificar_por_srt(void *_);

static void *manejar_desalojado(void *_);

void inicializar_planificador_corto_plazo(q_estado *q_ready, q_estado *q_exit)
{
    q_ready = q_ready;
    q_exit = q_exit;

    alpha = get_alfa_estimacion();
    estimacion_inicial = get_estimacion_inicial();

    void *(*planificador_corto_plazo)(void *) = NULL;

    switch (get_alg_plani_corto_plazo())
    {
    case FIFO:
        planificador_corto_plazo = &planificar_por_fifo;
        break;
    case SJF:
        planificador_corto_plazo = &planificar_por_sjf;
        break;
    case SRT:
        planificador_corto_plazo = &planificar_por_srt;
        break;
    default:
        log_mensaje_error("Algoritmo de planificación de corto plazo no soportado.");
        return;
    }
}

static double estimar_rafaga(double anterior_estimado, double real_anterior)
{
    return alpha * real_anterior + (1 - alpha) * anterior_estimado;
}

static void *planificar_por_fifo(void *_)
{
    while (1)
    {
        t_pcb *proceso = pop_proceso(q_ready);
        

        // push_proceso(cola_exec, proceso);

        // proceso = peek_proceso(cola_exec);
        // enviar_pcb_cpu(proceso);
        // t_pcb *pos_exec = recibir_pcb_cpu();

        // actualizar_pcb(proceso, pos_exec);
        // destruir_pcb(pos_exec);

        // proceso = remove_proceso(cola_exec, proceso->pid);
        // pasar_a_siguiente(proceso);
    }

    return NULL;
}

static void *planificar_por_sjf(void *_) {}

static void *planificar_por_srt(void *_) {}

static void *manejar_desalojado(void *_) {}