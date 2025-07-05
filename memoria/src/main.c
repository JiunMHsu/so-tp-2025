#include <signal.h>

#include "config/config.h"
#include "logger/logger.h"
#include "usuario/usuario.h"
#include "sistema/sistema.h"
#include "swap/swap.h"
#include "servidor/servidor.h"

int main(int argc, char *argv[])
{
    signal(SIGINT, &finalizar_servidor_por_sigint);

    iniciar_config();
    iniciar_logger(get_log_level());

    inicializar_memoria_usuario();
    inicializar_espacio_sistema();
    inicializar_swap();

    iniciar_servidor();

    return 0;
}
