#include "servidor.h"

int32_t fd_escucha_dispatch;
int32_t fd_escucha_interrupt;
int32_t fd_escucha_io;

static void *escuchar_cpu(void *_);
static void *escuchar_io(void *_);

void iniciar_servidor()
{
    pthread_t hilo_escucha_cpu;
    pthread_create(&hilo_escucha_cpu, NULL, &escuchar_cpu, NULL);
    pthread_detach(hilo_escucha_cpu);

    pthread_t hilo_escucha_io;
    pthread_create(&hilo_escucha_io, NULL, &escuchar_io, NULL);
    pthread_detach(hilo_escucha_io);
}

static void *escuchar_cpu(void *_)
{
    char *puerto_escucha_dispatch = get_puerto_escucha(ESCUCHA_CPU_DISPATCH);
    char *puerto_escucha_interrupt = get_puerto_escucha(ESCUCHA_CPU_INTERRUPT);

    fd_escucha_dispatch = crear_servidor(puerto_escucha_dispatch);
    fd_escucha_interrupt = crear_servidor(puerto_escucha_interrupt);

    log_evento("Escuchando CPU...");

    while (1)
    {
        int32_t fd_dispatch = esperar_cliente(fd_escucha_dispatch, NULL);
        if (recibir_cliente(fd_dispatch) != CPU)
        {
            log_mensaje_error("Error cliente inválido, cerrando conexión");
            cerrar_conexion(fd_dispatch);
            continue;
        }

        int32_t fd_interrupt = esperar_cliente(fd_escucha_interrupt, NULL);
        if (recibir_cliente(fd_interrupt) != CPU)
        {
            log_mensaje_error("Error cliente inválido, cerrando conexión");
            cerrar_conexion(fd_interrupt);
            cerrar_conexion(fd_dispatch);
            continue;
        }

        char *id_cpu = recibir_mensaje(fd_dispatch);
        if (id_cpu == NULL)
        {
            log_mensaje_error("Error al recibir ID de CPU, cerrando conexión");
            cerrar_conexion(fd_interrupt);
            cerrar_conexion(fd_dispatch);
            continue;
        }

        conectar_cpu(id_cpu, fd_dispatch, fd_interrupt);
        printf("CPU %s conectada\n", id_cpu);
        free(id_cpu);
    }

    return NULL;
}

static void *escuchar_io(void *_)
{
    char *puerto_escucha_io = get_puerto_escucha(ESCUCHA_IO);
    fd_escucha_io = crear_servidor(puerto_escucha_io);

    log_evento("Escuchando IO...");

    while (1)
    {
        int32_t fd_io = esperar_cliente(fd_escucha_io, NULL);
        if (recibir_cliente(fd_io) != IO)
        {
            log_mensaje_error("Error cliente inválido");
            cerrar_conexion(fd_io);
            return NULL;
        }

        char *nombre_io = recibir_mensaje(fd_io);
        if (nombre_io == NULL)
        {
            log_mensaje_error("Error al recibir nombre de IO, cerrando conexión");
            cerrar_conexion(fd_io);
            continue;
        }

        conectar_io(nombre_io, fd_io);
        printf("IO %s conectado\n", nombre_io);
        free(nombre_io);
    }

    return NULL;
}

void finalizar_servidor()
{
    printf("\n");
    log_evento("Finalizando servidor...");

    cerrar_conexion(fd_escucha_dispatch);
    cerrar_conexion(fd_escucha_interrupt);
    cerrar_conexion(fd_escucha_io);

    log_evento("Servidor finalizado.");
}

void finalizar_servidor_por_sigint(int _)
{
    finalizar_servidor();
    exit(EXIT_SUCCESS);
}
