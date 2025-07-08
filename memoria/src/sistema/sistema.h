#ifndef MEMORIA_SISTEMA_H
#define MEMORIA_SISTEMA_H

#include <stdio.h>
#include <stdlib.h>
#include <commons/string.h>
#include <commons/collections/dictionary.h>
#include <commons/collections/list.h>
#include <utils/mem_request/kernel.h>
#include <utils/string/string.h>

#include "logger/logger.h"
#include "estado_frame/estado_frame.h"
#include "metricas/metricas.h"
#include "tabla_paginas/tabla_paginas.h"

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

#endif // MEMORIA_SISTEMA_H
