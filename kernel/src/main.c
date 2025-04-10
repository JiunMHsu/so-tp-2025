#include <signal.h>
#include "servidor/servidor.h"

void sigint_handler(int);

int main(int argc, char *argv[])
{
    signal(SIGINT, &sigint_handler);
    // conectar con memoria

    iniciar_servidor();

    // planificador

    pause(); // temporal para bloquear el hilo principal

    return EXIT_SUCCESS;
}

void sigint_handler(int _)
{
    finalizar_servidor();
}
