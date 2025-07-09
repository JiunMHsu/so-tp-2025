#ifndef MEMORIA_USUARIO_H
#define MEMORIA_USUARIO_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "estado_frame/estado_frame.h"

void inicializar_espacio_usuario(void);

/**
 * @brief Escribe en la memoria de usuario según la dirección física y el buffer proporcionados.
 * 
 * @return `1` si la escritura fue exitosa, `0` en caso contrario.
 */
u_int8_t escribir_memoria_usuario(u_int32_t pid, u_int32_t direccion_fisica, void *buffer, u_int32_t tamanio_buffer);
void *leer_memoria_usuario(u_int32_t pid, u_int32_t direccion_fisica, u_int32_t tamanio_buffer);

#endif // MEMORIA_USUARIO_H
