#ifndef CONEXION_MEMORIA_H
#define CONEXION_MEMORIA_H

#include <stdlib.h>
#include <pthread.h>
#include <utils/sockets/sockets.h>
#include <utils/protocol/protocol.h>
#include <utils/mem_request/cpu.h>

#include "config/config.h"
#include "logger/logger.h"

int8_t conectar_memoria(void);
char *recibir_mensaje_memoria();
void enviar_peticion_instruccion(u_int32_t pid, u_int32_t program_counter);

// void enviar_peticion_nro_marco(u_int32_t pid, u_int32_t nro_pag);
// void enviar_peticion_lectura(u_int32_t pid, char *direcciones_fisicas, u_int32_t tamanio_buffer);
// void enviar_peticion_escritura(u_int32_t pid, char *direcciones_fisicas, u_int32_t tamanio_buffer, void *buffer);

// funciones de peticion a memoria (consumir utils/mem_request/cpu)

#endif // CONEXION_MEMORIA_H
