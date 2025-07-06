#ifndef MEMORIA_USUARIO_H
#define MEMORIA_USUARIO_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "estado_frame/estado_frame.h"

void inicializar_espacio_usuario(void);

u_int8_t escribir_memoria_usuario(u_int32_t pid, t_list *direcciones_fisicas, void *buffer, u_int32_t tamanio_buffer);
void *leer_memoria_usuario(u_int32_t pid, t_list *direcciones_fisicas, u_int32_t tamanio_buffer);

#endif // MEMORIA_USUARIO_H
