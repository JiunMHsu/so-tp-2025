#ifndef MEMORIA_SISTEMA_H
#define MEMORIA_SISTEMA_H

#include <stdio.h>
#include <stdlib.h>
#include <commons/string.h>
#include <commons/collections/dictionary.h>
#include <commons/collections/list.h>
#include <utils/mem_request/kernel.h>
#include <utils/string/string.h>

#include "config/config.h"
#include "logger/logger.h"
#include "estado_frame/estado_frame.h"
#include "metricas/metricas.h"
#include "tabla_paginas/tabla_paginas.h"
#include "usuario/usuario.h"
#include "swap/swap.h"
#include "dump/dump.h"

void inicializar_espacio_sistema(void);

/**
 * @brief Crea un proceso en memoria.
 *
 * @return `1` si se creó el proceso correctamente, `0` en caso contrario.
 */
u_int8_t crear_proceso(u_int32_t pid, u_int32_t tamanio, char *path);

/**
 * @brief Libera un proceso de memoria.
 *
 * @return `1` si se liberó correctamente, `0` en caso contrario.
 */
u_int8_t finalizar_proceso(u_int32_t pid);

char *obtener_instruccion(u_int32_t pid, u_int32_t program_counter);

/**
 * @brief Mueve un proceso a swap.
 *
 * @param pid
 * @return `1` si se pudo mover a swap correctamente, `0` en caso contrario.
 */
u_int8_t swap_out_proceso(u_int32_t pid);

/**
 * @brief Mueve un proceso de swap a memoria.
 *
 * @param pid
 * @return `1` si se pudo mover de swap a memoria correctamente, `0` en caso contrario.
 */
u_int8_t swap_in_proceso(u_int32_t pid);

/**
 * @brief Genera un archivo de dump del proceso.
 *
 * @param pid
 * @return `1` si se generó el dump correctamente, `0` en caso contrario.
 */
u_int8_t dump_proceso(u_int32_t pid);

#endif // MEMORIA_SISTEMA_H
