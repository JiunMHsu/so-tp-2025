#include "config/config.h"
#include "logger/logger.h"
#include "recursos/cpu.h"
#include "recursos/io.h"
#include "planificador/planificador.h"
#include "servidor/servidor.h"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Uso: %s <archivo_pseudocodigo> <tamanio_proceso> \n", argv[0]);
        return EXIT_FAILURE;
    }

    char *pseudocodigo = argv[1];
    u_int32_t tamanio_proceso = atoi(argv[2]);

    signal(SIGINT, &finalizar_servidor_por_sigint);

    iniciar_config();
    iniciar_logger(get_log_level());

    inicializar_cpu();
    inicializar_io();
    iniciar_servidor();

    inicializar_planificador(pseudocodigo, tamanio_proceso);

    return EXIT_SUCCESS;
}
