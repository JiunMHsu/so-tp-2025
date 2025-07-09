#ifndef MEMORIA_USUARIO_H
#define MEMORIA_USUARIO_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <utils/mem_response/mem_response.h>

#include "estado_frame/estado_frame.h"

void inicializar_espacio_usuario(void);

t_mem_response escribir_memoria_usuario(u_int32_t pid, u_int32_t direccion_fisica, void *buffer, u_int32_t tamanio_buffer);
void *leer_memoria_usuario(u_int32_t pid, u_int32_t direccion_fisica, u_int32_t tamanio_buffer);

#endif // MEMORIA_USUARIO_H
