#include "io.h"

// TODO: reemplazar por colecciones thread-safe (listas y colas con mutex)
t_list *ios;
t_queue *finalizados;

static t_io *crear_io(char *nombre_io, int32_t fd_io);
static void destruir_io(t_io *io);
static t_io *buscar_por_nombre(char *nombre_io);

static void *consumir_io(void *dispositivo_io);
static void finalizar_consumo_para(t_pcb *proceso, motivo_fin_io motivo);

static void desconectar_io(char *nombre_io);

// TODO: inicializar estructura de datos para manejar IOs
void inicializar_io()
{
}

// TODO: implementar
void conectar_io(char *nombre_io, int32_t fd_io)
{
    // crear t_io*
    // agregar a lista
    // crear hilo de consumo
}

// TODO: implementar
int32_t bloquear_para_io(char *nombre_io, t_pcb *pcb)
{
    return 0;
}

// TODO: implementar
t_fin_de_io *get_finalizado(void)
{
    return NULL;
}

void destruir_fin_de_io(t_fin_de_io *fin_de_io)
{
    free(fin_de_io);
}

static t_io *crear_io(char *nombre_io, int32_t fd_io)
{
    t_io *io = malloc(sizeof(t_io));
    io->fd_io = fd_io;
    io->nombre = strdup(nombre_io);
    io->cola_procesos = queue_create();
    io->rutina_consumo = 0;

    return io;
}

static void destruir_io(t_io *io)
{
    cerrar_conexion(io->fd_io);

    // no destruye los procesos
    queue_destroy(io->cola_procesos);
    free(io->nombre);
    free(io);
}

static t_io *buscar_por_nombre(char *nombre_io)
{
    bool _tiene_nombre(void *_io)
    {
        t_io *io = (t_io *)_io;
        return strcmp(io->nombre, nombre_io) == 0;
    };

    return (t_io *)list_find(ios, &_tiene_nombre);
}

// TODO: implementar
static void *consumir_io(void *dispositivo_io)
{
    t_io *io = (t_io *)dispositivo_io;

    while (1)
    {
        // esperar por un proceso
        // pop de la cola (io->cola_procesos)

        // empaquetar la petición a IO
        // enviar al socket de IO (io->fd_io)
        // esperar y recibir respuesta

        // manejar la respuesta
    }

    return NULL;
}

static void finalizar_consumo_para(t_pcb *proceso, motivo_fin_io motivo)
{
    t_fin_de_io *fin_de_io = malloc(sizeof(t_fin_de_io));
    fin_de_io->proceso = proceso;
    fin_de_io->motivo = motivo;

    // agregar a la cola de finalizados
    queue_push(finalizados, fin_de_io);
}

/**
 * @brief Función ideada para que la rutina de consumo llame en caso de
 * escuchar un -1 por el socket. Simplemente mueve los procesos a la cola
 * de finalizados y libera las estructuras, no cancela el hilo de ejecución.
 *
 * TODO: implementar
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