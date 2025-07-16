#ifndef RECURSOS_IO_H
#define RECURSOS_IO_H

#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <utils/sockets/sockets.h>
#include <utils/protocol/protocol.h>
#include <utils/io/peticion_io.h>
#include <utils/io/motivo_fin_io.h>
#include <utils/mlist/mlist.h>
#include <utils/mqueue/mqueue.h>

#include "config/config.h"
#include "logger/logger.h"
#include "pcb/pcb.h"
#include "planificador/plani_mediano_plazo.h"

typedef struct
{
    char *nombre;

    t_mutex_list *instancias;
    t_mutex_list *instancias_libres;
    sem_t *hay_instancia_libre;

    t_mutex_list *peticiones;
    sem_t *hay_peticion;

    pthread_t rutina_consumo;
} t_io;

typedef struct
{
    int32_t fd_io;
    t_peticion_io *peticion;
    sem_t *hay_peticion;
    t_pcb *proceso;
    pthread_t rutina_consumo;

    t_io *io; // referencia al IO al que pertenece esta instancia
} t_instancia_io;

typedef struct
{
    t_pcb *proceso;
    u_int32_t tiempo;
} t_peticion_consumo;

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
int32_t bloquear_para_io(char *nombre_io, t_pcb *proceso, u_int32_t tiempo);

#endif // RECURSOS_IO_H
