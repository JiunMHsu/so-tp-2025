#ifndef CONEXION_KERNEL_H
#define CONEXION_KERNEL_H

#include <stdlib.h>
#include <pthread.h>
#include <commons/string.h>
#include <commons/config.h>
#include <utils/sockets/sockets.h>
#include <utils/protocol/protocol.h>
#include <utils/ejecucion/peticion_ejecucion.h>
#include <utils/ejecucion/desalojo.h>

#include "config/config.h"
#include "logger/logger.h"

/**
 * @brief Conecta el CPU al kernel.
 *
 * @param id_cpu Identificador del CPU.
 *  @return 0 si la conexión fue exitosa, -1 en caso contrario.
 */
int8_t conectar_kernel(char *id_cpu);
void cerrar_conexion_kernel(void);

t_peticion_ejecucion *recibir_peticion_ejecucion_kernel(void);
int32_t recibir_interrupt(void);
void enviar_desalojo_kernel(u_int32_t pid, u_int32_t program_counter, motivo_desalojo motivo, char *syscall);

#endif // CONEXION_KERNEL_H
