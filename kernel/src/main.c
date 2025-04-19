#include "config/config.h"
#include "logger/logger.h"
#include "recursos/cpu.h"
#include "recursos/io.h"
#include "servidor/servidor.h"

int main(int argc, char *argv[])
{
    signal(SIGINT, &finalizar_servidor_por_sigint);

    iniciar_config();
    iniciar_logger(get_log_level());

    inicializar_cpu();
    inicializar_io();
    iniciar_servidor();
    // planificador

    pause(); // temporal para bloquear el hilo principal
    return EXIT_SUCCESS;
}
