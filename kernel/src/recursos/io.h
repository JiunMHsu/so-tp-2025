#ifndef RECURSOS_IO_H
#define RECURSOS_IO_H

#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <commons/collections/list.h>  // temporal (reemplazar por mlist)
#include <commons/collections/queue.h> // temporal (reemplazar por mqueue)
#include <utils/sockets/sockets.h>
#include <utils/protocol/protocol.h>
#include <utils/io/peticion_io.h>
#include <utils/io/motivo_fin_io.h>
// #include <utils/mlist/mlist.h>
// #include <utils/mqueue/mqueue.h>

#include "config/config.h"
#include "logger/logger.h"
#include "pcb/pcb.h"

typedef struct
{
    char *nombre;
    int32_t fd_io;
    t_queue *cola_procesos; // TODO: reemplazar por mqueue
    pthread_t rutina_consumo;
} t_io;

typedef struct
{
    t_pcb *proceso;
    motivo_fin_io motivo;
} t_fin_de_io;

/**
 * @brief Inicializa la colección global para manejar los IOs.
 *
 */
void inicializar_io(void);

void conectar_io(char *nombre_io, int32_t fd_io);

/**
 * @brief Encola un proceso a un dispositivo IO para su consumo.
 *
 * @param nombre_io: nombre del dispositivo IO a consumir
 * @param proceso: proceso a encolar
 *
 * @return int32_t : 0 si fue exitosa, -1 si hubo un error
 */
int32_t bloquear_para_io(char *nombre_io, t_pcb *proceso);

/**
 * @brief Escucha constantemente si hay procesos que terminaron su IO.
 *
 * @return t_fin_de_io*
 *
 * @note Es bloqueante.
 * @note Se debe liberar la estructura retornada con `destruir_fin_de_io`.
 */
t_fin_de_io *get_finalizado(void);

void destruir_fin_de_io(t_fin_de_io *fin_de_io);

#endif // RECURSOS_IO_H
