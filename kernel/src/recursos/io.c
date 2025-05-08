#include "io.h"

t_mutex_list *ios;

sem_t *hay_finalizado;
t_mutex_queue *finalizados;

static t_io *crear_io(char *nombre_io, int32_t fd_io);
static void destruir_io(t_io *io);
static t_io *buscar_por_nombre(char *nombre_io);

static t_peticion_consumo *crear_peticion_consumo(t_pcb *proceso, u_int32_t tiempo);
static void destruir_peticion_consumo(t_peticion_consumo *peticion_consumo);

static void *consumir_io(void *dispositivo_io);
static void finalizar_consumo_para(t_pcb *proceso, motivo_fin_io motivo);

static void desconectar_io(char *nombre_io);

void inicializar_io()
{
    ios = mlist_create();
    finalizados = mqueue_create();
    sem_init(hay_finalizado, 0, 0);
}

void conectar_io(char *nombre_io, int32_t fd_io)
{
    t_io *io = crear_io(nombre_io, fd_io);

    pthread_create(&(io->rutina_consumo), NULL, &consumir_io, io);
    pthread_detach(io->rutina_consumo);

    mlist_add(ios, io);
}

int32_t bloquear_para_io(char *nombre_io, t_pcb *pcb)
{
    t_io *io = buscar_por_nombre(nombre_io);
    if (io == NULL)
        return -1;

    mqueue_push(io->cola_procesos, pcb);
    sem_post(io->hay_proceso);
    return 0;
}

t_fin_de_io *get_finalizado(void)
{
    sem_wait(hay_finalizado);
    return (t_fin_de_io *)mqueue_pop(finalizados);
}

void destruir_fin_de_io(t_fin_de_io *fin_de_io)
{
    if (fin_de_io == NULL)
        return;

    free(fin_de_io);
    fin_de_io = NULL;
}

static t_io *crear_io(char *nombre_io, int32_t fd_io)
{
    t_io *io = malloc(sizeof(t_io));
    io->fd_io = fd_io;
    io->nombre = strdup(nombre_io);
    io->nombre = mqueue_create();
    sem_init(io->hay_peticion, 0, 0);
    io->rutina_consumo = 0;

    return io;
}

static void destruir_io(t_io *io)
{
    if (io == NULL)
        return;

    cerrar_conexion(io->fd_io);

    free(io->nombre);
    mqueue_destroy(io->peticiones);
    sem_destroy(io->hay_peticion);
    free(io);

    io = NULL;
}

static t_io *buscar_por_nombre(char *nombre_io)
{
    int32_t _tiene_nombre(void *_io)
    {
        t_io *io = (t_io *)_io;
        return strcmp(io->nombre, nombre_io) == 0;
    };

    return (t_io *)mlist_find(ios, &_tiene_nombre);
}

static t_peticion_consumo *crear_peticion_consumo(t_pcb *proceso, u_int32_t tiempo)
{
    t_peticion_consumo *peticion = malloc(sizeof(t_peticion_consumo));
    peticion->proceso = proceso;
    peticion->tiempo = tiempo;

    return peticion;
}

static void destruir_peticion_consumo(t_peticion_consumo *peticion_consumo)
{
    if (peticion_consumo == NULL)
        return;

    free(peticion_consumo);
    peticion_consumo = NULL;
}

static void *consumir_io(void *dispositivo_io)
{
    t_io *io = (t_io *)dispositivo_io;
    int32_t fd_io = io->fd_io;

    while (1)
    {
        sem_wait(io->hay_peticion);
        t_peticion_consumo *peticion = (t_peticion_consumo *)mqueue_pop(io->peticiones);
        t_peticion_io *peticion_io = crear_peticion_io(peticion->proceso->pid, peticion->tiempo);

        enviar_peticion_io(fd_io, peticion_io);

        destruir_peticion_consumo(peticion);
        destruir_peticion_io(peticion_io);

        int32_t respuesta_io = recibir_senial(fd_io);

        if (respuesta_io == -1)
        {
            // TODO: completar proceso desconexión io
            // desconectar_io(io->nombre);
            // finalizar_consumo_para(peticion->proceso, DISCONNECTED);
            // return NULL;
        }

        // TODO: completar proceso consumo exitoso
    }

    return NULL;
}

static void finalizar_consumo_para(t_pcb *proceso, motivo_fin_io motivo)
{
    t_fin_de_io *fin_de_io = malloc(sizeof(t_fin_de_io));
    fin_de_io->proceso = proceso;
    fin_de_io->motivo = motivo;

    mqueue_push(finalizados, fin_de_io);
    sem_post(hay_finalizado);
}

// TODO: implementar
/**
 * @brief Función ideada para que la rutina de consumo llame en caso de
 * escuchar un -1 por el socket. Simplemente mueve los procesos a la cola
 * de finalizados y libera las estructuras, no cancela el hilo de ejecución.
 *
 * @param nombre_io
 *
 */
static void desconectar_io(char *nombre_io)
{
    // remover io de la lista
    // pasar los procesos a la cola de finalizados {motivo DISCONNECTED}

    // destruir io
}