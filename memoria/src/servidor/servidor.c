#include "servidor.h"

int32_t fd_memoria;

void iniciar_servidor()
{
    char* puerto_escucha_cpu = config_get_string_value(config, "PUERTO_ESCUCHA");
    fd_memoria = crear_servidor(&puerto_escucha_cpu);

    pthread_t hilo_escucha_cpu;
    pthread_create(&hilo_escucha_cpu, NULL, &escuchar_cpu, &fd_memoria);
    pthread_detach(hilo_escucha_cpu);

}

void finalizar_servidor(void) 
{
    log_evento("Finalizando servidor...");

    cerrar_conexion(fd_memoria);

    log_evento("Servidor finalizado.");
}