#include "servidor.h"

int32_t fd_dispatch;
int32_t fd_interrupt;
int32_t fd_io;

static void *escuchar_cpu_dispatch(void *fd_escucha);
static void *escuchar_cpu_interrupt(void *fd_escucha);
static void *escuchar_io(void *fd_escucha);

void iniciar_servidor()
{
    char *puerto_escucha_dispatch = get_puerto_escucha(ESCUCHA_CPU_DISPATCH);
    char *puerto_escucha_interrupt = get_puerto_escucha(ESCUCHA_CPU_INTERRUPT);
    char *puerto_escucha_io = get_puerto_escucha(ESCUCHA_IO);

    fd_dispatch = crear_servidor(puerto_escucha_dispatch);
    fd_interrupt = crear_servidor(puerto_escucha_interrupt);
    fd_io = crear_servidor(puerto_escucha_io);

    pthread_t hilo_escucha_dispatch;
    pthread_create(&hilo_escucha_dispatch, NULL, &escuchar_cpu_dispatch, &fd_dispatch);
    pthread_detach(hilo_escucha_dispatch);

    pthread_t hilo_escucha_interrupt;
    pthread_create(&hilo_escucha_interrupt, NULL, &escuchar_cpu_interrupt, &fd_interrupt);
    pthread_detach(hilo_escucha_interrupt);

    pthread_t hilo_escucha_io;
    pthread_create(&hilo_escucha_io, NULL, &escuchar_io, &fd_io);
    pthread_detach(hilo_escucha_io);
}

static void *escuchar_cpu_dispatch(void *fd_escucha)
{
    log_evento("Escuchando CPU Dispatch...");
    while (1)
        esperar_cliente(*((int32_t *)fd_escucha), &manejar_conexion_dispatch);

    return NULL;
}

static void *escuchar_cpu_interrupt(void *fd_escucha)
{
    log_evento("Escuchando CPU Interrupt...");
    while (1)
        esperar_cliente(*((int32_t *)fd_escucha), &manejar_conexion_interrupt);

    return NULL;
}

void *escuchar_io(void *fd_escucha)
{
    log_evento("Escuchando IO...");
    while (1)
        esperar_cliente(*((int32_t *)fd_escucha), &manejar_conexion_io);

    return NULL;
}

void finalizar_servidor()
{
    printf("\n");
    log_evento("Finalizando servidor...");

    cerrar_conexion(fd_dispatch);
    cerrar_conexion(fd_interrupt);
    cerrar_conexion(fd_io);

    log_evento("Servidor finalizado.");
}
