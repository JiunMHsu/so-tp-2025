#include <signal.h>

#include "logger/logger.h"
#include "servidor/servidor.h"

int main(int argc, char *argv[])
{
    signal(SIGINT, &finalizar_servidor_por_sigint);

    iniciar_config();
    iniciar_logger(get_log_level());

    iniciar_servidor();

    pause(); // temporal para bloquear el hilo principal

    return 0;
}
