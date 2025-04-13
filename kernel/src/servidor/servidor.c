#include "servidor.h"

int32_t fd_dispatch;
int32_t fd_interrupt;
int32_t fd_io;

static void *escuchar_cpu_dispatch(void *fd_escucha);
static void *escuchar_cpu_interrupt(void *fd_escucha);
static void *escuchar_io(void *fd_escucha);

static void *atender_cpu_dispatch(void *fd_ptr);
static void *atender_cpu_interrupt(void *fd_ptr);
static void *atender_io(void *fd_ptr);

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
    // TODO: Reemplazar por logger
    printf("Escuchando CPU Dispatch en el socket %d ...\n", *((int32_t *)fd_escucha));

    while (1)
        esperar_cliente(*((int32_t *)fd_escucha), &atender_cpu_dispatch);

    return NULL;
}

static void *escuchar_cpu_interrupt(void *fd_escucha)
{
    // TODO: Reemplazar por logger
    printf("Escuchando CPU Interrupt en el socket %d ...\n", *((int32_t *)fd_escucha));

    while (1)
        esperar_cliente(*((int32_t *)fd_escucha), &atender_cpu_interrupt);

    return NULL;
}

void *escuchar_io(void *fd_escucha)
{
    // TODO: Reemplazar por logger
    printf("Escuchando IO en el socket %d ...\n", *((int32_t *)fd_escucha));

    while (1)
        esperar_cliente(*((int32_t *)fd_escucha), &atender_io);

    return NULL;
}

static void *atender_cpu_dispatch(void *fd_ptr)
{
    int32_t fd_cpu_dispatch = *((int32_t *)fd_ptr);
    free(fd_ptr);

    if (recibir_cliente(fd_cpu_dispatch, CPU_DISPATCH))
    {
        // TODO: Reemplazar por logger
        perror("Error cliente inválido");
        return NULL;
    }

    // TODO: Lógica de atención
    while (1)
    {
        enviar_mensaje("mensaje desde kernel", fd_cpu_dispatch);
        sleep(2);
    }

    return NULL;
}

static void *atender_cpu_interrupt(void *fd_ptr)
{
    int32_t fd_cpu_interrupt = *((int32_t *)fd_ptr);
    free(fd_ptr);

    if (recibir_cliente(fd_cpu_interrupt, CPU_INTERRUPT))
    {
        // TODO: Reemplazar por logger
        perror("Error cliente inválido");
        return NULL;
    }

    // TODO: Lógica de atención
    while (1)
    {
        enviar_mensaje("mensaje desde kernel", fd_cpu_interrupt);
        sleep(2);
    }

    return NULL;
}

static void *atender_io(void *fd_ptr)
{
    int32_t fd_io = *((int32_t *)fd_ptr);
    free(fd_ptr);

    if (recibir_cliente(fd_io, IO))
    {
        // TODO: Reemplazar por logger
        perror("Error cliente inválido");
        return NULL;
    }

    // TODO: Lógica de atención
    while (1)
    {
        enviar_mensaje("mensaje desde kernel", fd_io);
        sleep(2);
    }

    return NULL;
}

void finalizar_servidor()
{
    printf("Finalizando servidor...\n");

    cerrar_conexion(fd_dispatch);
    cerrar_conexion(fd_interrupt);
    cerrar_conexion(fd_io);

    printf("Servidor finalizado.\n");
}
