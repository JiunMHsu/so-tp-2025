#include "planificador.h"

static sem_t *fin_planificador;

static void esperar_enter();

void inicializar_planificador(char *archivo_pseudocodigo, u_int32_t tamanio_proceso)
{
    fin_planificador = malloc(sizeof(sem_t));
    sem_init(fin_planificador, 0, 0);

    esperar_enter();

    algoritmo_planificacion algoritmo_corto_plazo = get_alg_plani_corto_plazo();
    algoritmo_planificacion algoritmo_ingreso_a_ready = get_alg_ingreso_a_ready();

    q_estado *q_new = crear_estado();
    q_estado *q_ready = crear_estado();
    q_estado *q_exit = crear_estado();
    q_estado *q_susp_ready = crear_estado();

    inicializar_planificador_largo_plazo(algoritmo_ingreso_a_ready, q_new, q_ready, q_exit);
    inicializar_planificador_mediano_plazo(q_susp_ready, q_ready, q_exit);
    inicializar_planificador_corto_plazo(algoritmo_corto_plazo, q_ready);

    sem_wait(fin_planificador);
}

static void esperar_enter()
{
    char *input = readline("[+] Presione Enter para comenzar...\n");
    while (input[0] != '\0')
    {
        free(input);
        input = readline("[+] Presione Enter para continuar...\n");
    }

    free(input);
}
