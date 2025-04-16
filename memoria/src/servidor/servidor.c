#include "servidor.h"

int32_t fd_memoria;

int32_t iniciar_servidor()
{
    char *puerto_escucha = get_puerto_escucha();
    fd_memoria = crear_servidor(puerto_escucha);
    return fd_memoria;
}

void finalizar_servidor(void)
{
    log_evento("Finalizando servidor...");

    cerrar_conexion(fd_memoria);

    log_evento("Servidor finalizado.");
}