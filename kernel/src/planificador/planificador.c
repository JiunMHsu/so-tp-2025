#include "planificador.h"



void inicializar_planificador(char *archivo_pseudocodigo, u_int32_t tamanio_proceso)
{
    //semaforo binario para ordenar
    sem_init(&iniciar_planificacion, 0, 0);
    //porque solo quiero leer la cadena, puntero a la misma direccion
    ALGORITMO_A_USAR = archivo_pseudocodigo;

    printf("Presione Enter para iniciar la planificación...\n");
    //TODO que espere el enter 

    NEW = queue_create();
    READY = queue_create();

    sem_post(&iniciar_planificacion);

    //hacer sem_destroy donde corresponda
}

