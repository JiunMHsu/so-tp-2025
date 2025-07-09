#include "usuario.h"

static void *memoria_usuario;
static pthread_mutex_t memoria_usuario_mutex;

void inicializar_espacio_usuario()
{
    memoria_usuario = malloc(get_tam_memoria());
    pthread_mutex_init(&memoria_usuario_mutex, NULL);
}

u_int8_t escribir_memoria_usuario(u_int32_t pid, t_list *direcciones_fisicas, void *buffer, u_int32_t tamanio_buffer)
{
}

void *leer_memoria_usuario(u_int32_t pid, t_list * direcciones_fisicas, u_int32_t tamanio_buffer)
{
}

void *leer_pagina_completa(u_int32_t pid, u_int32_t frame)
{
}

u_int8_t *actulizar_pagina_completa(u_int32_t pid, u_int32_t frame, void *buffer)
{
}