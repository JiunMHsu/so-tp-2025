#include "planificador.h"

static sem_t *fin_planificador;

static void esperar_enter();

void inicializar_planificador(char *archivo_pseudocodigo, u_int32_t tamanio_proceso)
{
    fin_planificador = malloc(sizeof(sem_t));
    sem_init(fin_planificador, 0, 0);

    esperar_enter();

    q_estado *q_new = crear_estado(NEW);
    q_estado *q_ready = crear_estado(READY);
    q_estado *q_executing = crear_estado(EXEC);
    q_estado *q_blocked = crear_estado(BLOCKED);
    q_estado *q_susp_blocked = crear_estado(SUSPENDED_BLOCKED);
    q_estado *q_susp_ready = crear_estado(SUSPENDED_READY);
    q_estado *q_exit = crear_estado(EXIT);

    inicializar_planificador_largo_plazo(q_new, q_exit);
    inicializar_planificador_mediano_plazo(q_blocked, q_susp_blocked, q_susp_ready);
    inicializar_planificador_corto_plazo(q_ready, q_executing);

    insertar_proceso_nuevo(archivo_pseudocodigo, tamanio_proceso);

    sem_wait(fin_planificador);
}

static void esperar_enter()
{
    char *input = readline("[+] Presione Enter para comenzar...\n");
    while (input[0] != '\0')
    {
        free(input);
        input = readline("[+] Presione Enter para comenzar...\n");
    }

    free(input);
}
