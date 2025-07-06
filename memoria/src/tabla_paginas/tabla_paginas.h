#ifndef TABLA_PAGINAS_H
#define TABLA_PAGINAS_H

#include <stdlib.h>
#include <pthread.h>
#include <commons/collections/list.h>
#include <commons/collections/dictionary.h>

#include "config/config.h"
#include "logger/logger.h"

typedef struct
{
    /**
     * @brief Nivel de la tabla de páginas. 1 a n.
     */
    u_int32_t nivel;
    t_list *entradas;
} t_tabla;

typedef struct
{
    /**
     * @brief `1` si la entrada está cargada, `0` si no.
     */
    u_int8_t presente;

    /**
     * @brief Puntero a la siguiente tabla de nivel inferior.
     * @note `NULL` si es el último nivel de la tabla.
     */
    t_tabla *siguiente;

    /**
     * @brief Número de marco asignado a la entrada.
     * @note `-1`si no tiene asignado un marco.
     */
    int32_t marco;
} t_entrada;

void inicializar_tabla_de_paginas(void);
void crear_tablas_para(u_int32_t pid);
void destruir_tablas_para(u_int32_t pid);

#endif // TABLA_PAGINAS_H
