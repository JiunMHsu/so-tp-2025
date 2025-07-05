#ifndef ESTADO_FRAME_H
#define ESTADO_FRAME_H

#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <commons/bitarray.h>
#include <pthread.h>
#include <math.h>

#include "config/config.h"
#include "logger/logger.h"

typedef enum
{
   LIBRE,
   OCUPADO
} t_estado_frame;

/**
 * @brief Inicializa el bitmap de estados.
 */
void inicializar_bitmap_estados(void);

/**
 * @brief Modifica el estado de un frame en el bitmap (LIBRE / OCUPADO).
 *
 * @param frame
 * @param estado `t_estado_frame` - Estado del frame (0: LIBRE, 1: OCUPADO).
 */
void set_estado_frame(u_int32_t frame, t_estado_frame estado);

/**
 * @brief Retorna el primer frame libre según el bitmap.
 *
 * @return `u_int32_t`
 * @note Si no hay frames libres, retorna -1.
 */
int32_t get_frame_libre(void);

/**
 * @brief Retorna la cantidad de frames disponibles (Libres).
 *
 * @return `u_int32_t`
 */
u_int32_t get_cantidad_frames_disponibles(void);

/**
 * @brief Destruye el bitmap de estados.
 */
void destruir_bitmap_estados(void);

#endif // ESTADO_FRAME_H
