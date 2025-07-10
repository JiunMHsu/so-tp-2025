#ifndef CONEXION_MEMORIA_H
#define CONEXION_MEMORIA_H

#include <stdlib.h>
#include <pthread.h>
#include <utils/sockets/sockets.h>
#include <utils/protocol/protocol.h>
#include <utils/mem_request/cpu.h>
#include <utils/mem_response/mem_response.h>
#include <utils/mem_datos_paginacion/mem_datos_paginacion.h>

#include "config/config.h"
#include "logger/logger.h"

int8_t conectar_memoria(void);
void cerrar_conexion_memoria(void);

char *recibir_mensaje_memoria(void);
u_int32_t recibir_marco(void);
void *recibir_contenido_pagina(void);
void *recibir_datos_lectura(void);
u_int32_t get_cantidad_niveles(void);
u_int32_t get_cantidad_entradas_tp(void);
u_int32_t get_tamanio_pagina(void);
void enviar_peticion_instruccion(u_int32_t pid, u_int32_t program_counter);
void enviar_peticion_marco(u_int32_t pid, char *entradas_por_nivel);
void enviar_peticion_escritura(u_int32_t pid, u_int32_t direccion_fisica, void *contenido, u_int32_t tamanio_bytes);
void enviar_peticion_lectura(u_int32_t pid, u_int32_t direccion_fisica, u_int32_t tamanio_bytes);
void enviar_peticion_escritura_pagina(u_int32_t pid, u_int32_t direccion_fisica, void *contenido);
void enviar_peticion_lectura_pagina(u_int32_t pid, u_int32_t direccion_fisica);

#endif // CONEXION_MEMORIA_H
