#include "io.h"

static t_mutex_list *ios;

static sem_t *hay_finalizado;
static t_mutex_queue *finalizados;

static t_io *crear_io(char *nombre_io);
static void desconectar_io(char *nombre_io);
static void destruir_io(t_io *io);
static t_io *buscar_por_nombre(char *nombre_io);

static void desconectar_instancia_io(t_io *io, int32_t fd_io);

static t_instancia_io *crear_instancia_io(int32_t fd_io);
static void conectar_instancia_io(t_io *io, t_instancia_io *instancia_io);
static void set_instancia_libre(t_io *io, t_instancia_io *instancia_io);
static void _cerrar_conexion_instancia_io(void *instancia_io);
static void destruir_instancia_io(t_instancia_io *instancia_io);

static t_peticion_consumo *crear_peticion_consumo(t_pcb *proceso, u_int32_t tiempo);
static void destruir_peticion_consumo(t_peticion_consumo *peticion_consumo);

static void *consumir_io(void *dispositivo_io);
static void *consumir_instancia_io(void *instancia_io);
static void finalizar_consumo_para(t_pcb *proceso, motivo_fin_io motivo);
static void _encolar_a_finalizados(void *_peticion_consumo);

static t_fin_de_io *get_finalizado(void);
static void destruir_fin_de_io(t_fin_de_io *fin_de_io);

static void *manejar_finalizados(void *_);

void inicializar_io()
{
    ios = mlist_create();
    finalizados = mqueue_create();
    hay_finalizado = malloc(sizeof(sem_t));
    sem_init(hay_finalizado, 0, 0);

    pthread_t rutina_manejo_finalizados;
    pthread_create(&rutina_manejo_finalizados, NULL, &manejar_finalizados, NULL);
    pthread_detach(rutina_manejo_finalizados);
}

void conectar_io(char *nombre_io, int32_t fd_io)
{
    t_instancia_io *instancia = crear_instancia_io(fd_io);
    t_io *io = buscar_por_nombre(nombre_io);
    if (io != NULL) // TODO: verificar que buscar_por_nombre retorne NULL si no existe
    {
        conectar_instancia_io(io, instancia);
        return;
    }

    io = crear_io(nombre_io);
    conectar_instancia_io(io, instancia);

    pthread_create(&(io->rutina_consumo), NULL, &consumir_io, io);
    pthread_detach(io->rutina_consumo);

    mlist_add(ios, io);
}

int32_t bloquear_para_io(char *nombre_io, t_pcb *proceso, u_int32_t tiempo)
{
    t_io *io = buscar_por_nombre(nombre_io);
    if (io == NULL)
        return -1;

    t_peticion_consumo *peticion = crear_peticion_consumo(proceso, tiempo);
    mlist_push_as_queue(io->peticiones, peticion);
    log_motivo_bloqueo(proceso->pid, nombre_io);
    sem_post(io->hay_peticion);
    return 0;
}

static t_io *crear_io(char *nombre_io)
{
    t_io *io = malloc(sizeof(t_io));
    io->nombre = strdup(nombre_io);

    io->instancias = mlist_create();
    io->instancias_libres = mlist_create();
    io->hay_instancia_libre = malloc(sizeof(sem_t));
    sem_init(io->hay_instancia_libre, 0, 0);

    io->peticiones = mlist_create();
    io->hay_peticion = malloc(sizeof(sem_t));
    sem_init(io->hay_peticion, 0, 0);
    io->rutina_consumo = 0;

    return io;
}

static void desconectar_io(char *nombre_io)
{
    int32_t _tiene_nombre(void *_io)
    {
        t_io *io = (t_io *)_io;
        return strcmp(io->nombre, nombre_io) == 0;
    };

    t_io *io = mlist_remove_by_condition(ios, &_tiene_nombre);
    if (io == NULL)
        return;

    pthread_cancel(io->rutina_consumo);
    sem_post(io->hay_instancia_libre); // para que no quede bloqueada la rutina de consumo

    // igual no deberían estar todas desconectadas ya
    mlist_iterate(io->instancias, &_cerrar_conexion_instancia_io);
    mlist_iterate(io->peticiones, &_encolar_a_finalizados);

    destruir_io(io);
}

static void destruir_io(t_io *io)
{
    if (io == NULL)
        return;

    free(io->nombre);

    mlist_iterate(io->instancias, (void (*)(void *))destruir_instancia_io);
    mlist_destroy(io->instancias);

    mlist_iterate(io->instancias_libres, (void (*)(void *))destruir_instancia_io);
    mlist_destroy(io->instancias_libres);
    sem_destroy(io->hay_instancia_libre);

    mlist_iterate(io->peticiones, (void (*)(void *))destruir_peticion_consumo);
    mlist_destroy(io->peticiones);
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

static void desconectar_instancia_io(t_io *io, int32_t fd_io)
{
    int32_t _tiene_fd(void *_instancia_io)
    {
        t_instancia_io *instancia_io = (t_instancia_io *)_instancia_io;
        return instancia_io->fd_io == fd_io;
    };

    t_instancia_io *instancia_io = mlist_remove_by_condition(io->instancias, &_tiene_fd);
    if (instancia_io == NULL)
        return;

    _cerrar_conexion_instancia_io(instancia_io);
    destruir_instancia_io(instancia_io);

    if (mlist_is_empty(io->instancias))
        desconectar_io(io->nombre);
}

static t_instancia_io *crear_instancia_io(int32_t fd_io)
{
    t_instancia_io *instancia = malloc(sizeof(t_instancia_io));

    instancia->fd_io = fd_io;
    instancia->peticion = NULL;
    instancia->hay_peticion = malloc(sizeof(sem_t));
    sem_init(instancia->hay_peticion, 0, 0);
    instancia->proceso = NULL;
    instancia->rutina_consumo = 0;
    instancia->io = NULL; // se va a setear cuando se conecta

    return instancia;
}

static void conectar_instancia_io(t_io *io, t_instancia_io *instancia_io)
{
    instancia_io->io = io; // Asignar referencia al IO

    mlist_add(io->instancias, instancia_io);
    mlist_push_as_queue(io->instancias_libres, instancia_io);

    pthread_create(&(instancia_io->rutina_consumo), NULL, &consumir_instancia_io, instancia_io);
    pthread_detach(instancia_io->rutina_consumo);

    sem_post(io->hay_instancia_libre);
}

static void set_instancia_libre(t_io *io, t_instancia_io *instancia_io)
{
    // cuando llegue a este punto, debería ser NULL ya, pero es para asegurar
    instancia_io->peticion = NULL;
    mlist_push_as_queue(io->instancias_libres, instancia_io);
    sem_post(io->hay_instancia_libre);
}

static void _cerrar_conexion_instancia_io(void *instancia_io)
{
    t_instancia_io *instancia = (t_instancia_io *)instancia_io;
    if (instancia == NULL)
        return;

    cerrar_conexion(instancia->fd_io);
}

static void destruir_instancia_io(t_instancia_io *instancia_io)
{
    if (instancia_io == NULL)
        return;

    destruir_peticion_io(instancia_io->peticion); // TODO: remover si causa problemas
    sem_destroy(instancia_io->hay_peticion);
    free(instancia_io);
    instancia_io = NULL;
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
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    t_io *io = (t_io *)dispositivo_io;

    while (1)
    {
        sem_wait(io->hay_instancia_libre);
        pthread_testcancel();

        t_instancia_io *instancia = (t_instancia_io *)mlist_pop_as_queue(io->instancias_libres);

        sem_wait(io->hay_peticion);
        t_peticion_consumo *peticion = (t_peticion_consumo *)mlist_pop_as_queue(io->peticiones);

        t_pcb *proceso = peticion->proceso;
        u_int32_t tiempo = peticion->tiempo;
        destruir_peticion_consumo(peticion);

        t_peticion_io *peticion_io = crear_peticion_io(proceso->pid, tiempo);
        instancia->peticion = peticion_io;
        instancia->proceso = proceso;

        sem_post(instancia->hay_peticion);
    }

    return NULL;
}

static void *consumir_instancia_io(void *instancia_io)
{
    t_instancia_io *instancia = (t_instancia_io *)instancia_io;
    int32_t fd_io = instancia->fd_io;
    t_io *io = instancia->io;

    while (1)
    {
        sem_wait(instancia->hay_peticion);

        t_peticion_io *peticion_io = instancia->peticion;
        enviar_peticion_io(fd_io, peticion_io);
        destruir_peticion_io(peticion_io);

        int32_t respuesta_io = recibir_senial(fd_io);
        if (respuesta_io == EXECUTED)
        {
            finalizar_consumo_para(instancia->proceso, EXECUTED);
            set_instancia_libre(io, instancia);
            continue;
        }

        // caso -1 (o cualquier otra respuesta que no sea EXECUTED)
        finalizar_consumo_para(instancia->proceso, DISCONNECTED);
        break;
    }

    desconectar_instancia_io(io, fd_io);
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

static void _encolar_a_finalizados(void *_peticion_consumo)
{
    t_peticion_consumo *peticion_consumo = (t_peticion_consumo *)_peticion_consumo;
    finalizar_consumo_para(peticion_consumo->proceso, DISCONNECTED);
    destruir_peticion_consumo(peticion_consumo);
}

static t_fin_de_io *get_finalizado()
{
    sem_wait(hay_finalizado);
    return (t_fin_de_io *)mqueue_pop(finalizados);
}

static void destruir_fin_de_io(t_fin_de_io *fin_de_io)
{
    if (fin_de_io == NULL)
        return;

    free(fin_de_io);
    fin_de_io = NULL;
}

static void *manejar_finalizados(void *_)
{
    while (1)
    {
        t_fin_de_io *fin_de_io = get_finalizado();

        if (fin_de_io->motivo == EXECUTED)
            desbloquear_proceso(fin_de_io->proceso, 0);
        else
            desbloquear_proceso(fin_de_io->proceso, 1);

        destruir_fin_de_io(fin_de_io);
    }

    return NULL;
}
