#ifndef ESTADO_FRAME_H
#define ESTADO_FRAME_H

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <commons/bitarray.h>
#include <commons/collections/list.h>

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
 * @brief Busca y ocupa una cantidad de frames libres.
 * Si no hay suficientes frames libres, retorna NULL.
 *
 * @param cantidad_frames
 * @return t_list*
 */
t_list *ocupar_frames(u_int32_t cantidad_frames);

/**
 * @brief Libera los frames ocupados.
 * 
 * @param frames 
 */
void liberar_frames(t_list *frames);

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
