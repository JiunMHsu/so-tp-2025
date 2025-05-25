#include "plani_corto_plazo.h"

static q_estado *ready;

void inicializar_planificador_corto_plazo(algoritmo_planificacion alg_planificacion,
                                          q_estado *estado_ready)
{
    ready = estado_ready;
}

// funciones rutina
