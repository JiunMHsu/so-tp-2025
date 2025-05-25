#include "config/config.h"
#include "logger/logger.h"
#include "recursos/cpu.h"
#include "recursos/io.h"
#include "planificador/planificador.h"
#include "servidor/servidor.h"

int main(int argc, char *argv[])
{
    // TODO: Leer por argumentos el archivo de pseudocodigo y el tamaño del proceso
    char *pseudocodigo = "";
    u_int32_t tamanio_proceso = 0;

    signal(SIGINT, &finalizar_servidor_por_sigint);

    iniciar_config();
    iniciar_logger(get_log_level());

    inicializar_cpu();
    inicializar_io();
    iniciar_servidor();

    inicializar_planificador(pseudocodigo, tamanio_proceso);

    return EXIT_SUCCESS;
}
