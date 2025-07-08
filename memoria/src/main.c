#include <signal.h>

#include "config/config.h"
#include "logger/logger.h"
#include "usuario/usuario.h"
#include "sistema/sistema.h"
#include "swap/swap.h"
#include "servidor/servidor.h"

int main()
{
    signal(SIGINT, &finalizar_servidor_por_sigint);

    iniciar_config();
    iniciar_logger(get_log_level());

    inicializar_espacio_sistema();
    inicializar_espacio_usuario();
    inicializar_swap();

    iniciar_servidor();

    return EXIT_SUCCESS;
}
