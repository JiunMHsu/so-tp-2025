#include "plani_mediano_plazo.h"

static q_estado *q_susp_ready;
static q_estado *q_susp_blocked;
static q_estado *blocked;

static void suspender_proceso(t_pcb *proceso);
static void desuspender_proceso(t_pcb *proceso);

void inicializar_planificador_mediano_plazo(q_estado *q_susp_ready,
                                            q_estado *q_susp_blocked,
                                            q_estado *blocked)
{
}

// TODO: implementar inserción en blocked
void insertar_en_blocked(t_pcb *proceso) {}

void desbloquear_proceso(u_int32_t pid, int8_t resultado) {}

static void suspender_proceso(t_pcb *proceso) {}

static void desuspender_proceso(t_pcb *proceso) {}
