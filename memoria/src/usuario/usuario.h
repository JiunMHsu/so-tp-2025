#ifndef MEMORIA_USUARIO_H
#define MEMORIA_USUARIO_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "estado_frame/estado_frame.h"

void inicializar_espacio_usuario(void);

u_int8_t escribir_memoria_usuario(u_int32_t pid, u_int32_t direccion_fisica, void *buffer, u_int32_t tamanio_buffer);
void *leer_memoria_usuario(u_int32_t pid, u_int32_t direccion_fisica, u_int32_t tamanio_buffer);

void *leer_pagina_por_marco(u_int32_t marco);
void escribir_marco_entero(u_int32_t marco, void *contenido);

/**
 * @brief
 *
 * @param marcos
 * @return t_list*
 * @note Crea una lista de páginas leídas de los marcos proporcionados.
 * @note La lista debe ser destruida por el llamador.
 */
t_list *leer_paginas_por_marcos(t_list *marcos);

#endif // MEMORIA_USUARIO_H
