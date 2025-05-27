#include "plani_corto_plazo.h"

static q_estado *q_ready;
static q_estado *q_exit;

static double alpha;
static double estimacion_inicial;

double estimar_rafaga(double anterior_estimado, double real_anterior);

static void *planificar_por_fifo(void *_);
static void *planificar_por_sjf(void *_);
static void *planificar_por_srt(void *_);

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

double estimar_rafaga(double anterior_estimado, double real_anterior)
{
    return alpha * real_anterior + (1 - alpha) * anterior_estimado;
}

static void *planificar_por_fifo(void *_) {}

static void *planificar_por_sjf(void *_) {}

static void *planificar_por_srt(void *_) {}
