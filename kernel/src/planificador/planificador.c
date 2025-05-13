#include "planificador.h"

sem_t *fin_planificador;

void inicializar_planificador(char *archivo_pseudocodigo, u_int32_t tamanio_proceso)
{
    fin_planificador = malloc(sizeof(sem_t));
    sem_init(fin_planificador, 0, 0);

    // TODO: Implementar lectura de entrada estandar con readline (Espera del ENTER)
    printf("Presione Enter para iniciar la planificación...\n"); 

    // porque solo quiero leer la cadena, puntero a la misma direccion
    algoritmo_planificacion algoritmo_corto_plazo = get_alg_plani_corto_plazo();
    algoritmo_planificacion algoritmo_ingreso_a_ready = get_alg_ingreso_a_ready();

    q_estado *new = crear_estado();
    q_estado *ready = crear_estado();
    q_estado *exit = crear_estado();
    q_estado *susp_ready = crear_estado();

    sem_wait(fin_planificador);
}
