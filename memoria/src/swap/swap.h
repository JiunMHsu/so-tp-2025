#ifndef SWAP_H
#define SWAP_H

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <commons/collections/list.h>
#include <commons/collections/dictionary.h>

#include "config/config.h"
#include "logger/logger.h"

typedef struct
{
    u_int32_t paginas;
    u_int32_t posicion;
} t_swapped;

/**
 * @brief Inicializa las estructuras necesarias para el manejo de swap.
 * Crea el archivo de swap vacío.
 *
 */
void inicializar_swap();

/**
 * @brief Recibe el PID de un proceso y una lista de páginas (contenido).
 * Guarda las páginas en el archivo de swap y actualiza la información
 * del proceso en un diccionario.
 *
 * @param pid
 * @param paginas
 * @note Asume que la cantidad de páginas son estáticas, no icrementan ni decrementan.
 */
void guardar_en_swap(u_int32_t pid, t_list *paginas);

/**
 * @brief Recupera un proceso de swap y lo retorna las páginas en una lista, ordenada.
 *
 * @param pid
 * @return t_list*
 * @note Aloca memoria tanto para la lista como para las páginas dentro de ella. Debe ser liberada por el llamador.
 */
t_list *recuperar_de_swap(u_int32_t pid);

#endif // SWAP_H
