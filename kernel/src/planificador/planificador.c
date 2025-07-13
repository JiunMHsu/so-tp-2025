#include "planificador.h"

static sem_t *fin_planificador;

static void esperar_enter();

void inicializar_planificador(char *archivo_pseudocodigo, u_int32_t tamanio_proceso)
{
    fin_planificador = malloc(sizeof(sem_t));
    sem_init(fin_planificador, 0, 0);

    inicializar_planificador_largo_plazo();
    inicializar_planificador_mediano_plazo();
    inicializar_planificador_corto_plazo();

    esperar_enter();

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
