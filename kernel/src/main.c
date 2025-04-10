#include <semaphore.h>
#include "servidor/servidor.h"

int main(int argc, char *argv[])
{
    sem_t fin_de_proceso; // temporal para bloquear el hilo principal
    sem_init(&fin_de_proceso, 0, 0);

    // conectar con memoria

    iniciar_servidor();

    // planificador

    sem_wait(&fin_de_proceso);
    return EXIT_SUCCESS;
}
