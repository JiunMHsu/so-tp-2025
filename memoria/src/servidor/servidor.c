#include "servidor.h"

int32_t fd_memoria;

static void *escuchar_conexiones(void *_);

void iniciar_servidor()
{
    pthread_t hilo_escucha;
    pthread_create(&hilo_escucha, NULL, &escuchar_conexiones, NULL);
    pthread_detach(hilo_escucha);
}

void finalizar_servidor(void)
{
    log_evento("Finalizando servidor...");
    cerrar_conexion(fd_memoria);
    log_evento("Servidor finalizado.");
}

void finalizar_servidor_por_sigint(int _)
{
    finalizar_servidor();
    exit(EXIT_SUCCESS);
}

static void *escuchar_conexiones(void *_)
{
    char *puerto_escucha = get_puerto_escucha();
    fd_memoria = crear_servidor(puerto_escucha);

    while (1)
    {
        int32_t fd_cliente = esperar_cliente(fd_memoria, NULL);

        int8_t modulo_cliente = recibir_cliente(fd_cliente);
        switch (modulo_cliente)
        {
        case KERNEL:
            log_evento("Kernel conectado.");
            break;
        case CPU:
            log_evento("CPU conectado.");
            break;
        default:
            log_mensaje_error("Modulo desconocido.");
            finalizar_servidor();
            exit(EXIT_FAILURE);
        }
    }

    return NULL;
}
