#include "planificador.h"
#include <readline/readline.h>

sem_t *fin_planificador;

static void esperar_enter();

void inicializar_planificador(char *archivo_pseudocodigo, u_int32_t tamanio_proceso)
{
    fin_planificador = malloc(sizeof(sem_t));
    sem_init(fin_planificador, 0, 0);

    
    esperar_enter();


    algoritmo_planificacion algoritmo_corto_plazo = get_alg_plani_corto_plazo();
    algoritmo_planificacion algoritmo_ingreso_a_ready = get_alg_ingreso_a_ready();

    q_estado *new = crear_estado();
    q_estado *ready = crear_estado();
    q_estado *exit = crear_estado();
    q_estado *susp_ready = crear_estado();

    // TODO: Implementar inicializacion del planificador mediano plazo
    inicializar_planificador_corto_plazo(algoritmo_corto_plazo, ready);
    inicializar_planificador_largo_plazo(algoritmo_ingreso_a_ready, new, ready, exit);


    sem_wait(fin_planificador);
}

static void esperar_enter() 
{
    char *input;

    do 
    {
        input = readline("Presione Enter para comenzar...\n");

        // Para que no rompa si devuelve null
        if (input == NULL) {
            break;
        }

    } while (input[0] != '\0');  

    free(input); 
}

