#include "memoria.h"

static int32_t fd_memoria;

int8_t conectar_memoria()
{
    mem_address datos_memoria = get_memoria_address();
    fd_memoria = crear_conexion(datos_memoria.ip, datos_memoria.puerto);
    int32_t respuesta_conexion_memoria = handshake(fd_memoria, CPU);

    if (respuesta_conexion_memoria == -1)
    {
        log_mensaje_error("No se puedo establecer la conexion con la memoria");
        cerrar_conexion(fd_memoria);
        return -1;
    }

    return 0;
}

void cerrar_conexion_memoria()
{
    log_mensaje_error("Cerrando conexión con memoria...");
    cerrar_conexion(fd_memoria);
}

char *recibir_mensaje_memoria()
{
    return recibir_mensaje(fd_memoria);
}

u_int32_t recibir_marco()
{
    return recibir_senial(fd_memoria);
}

void *recibir_datos_lectura()
{
    t_mem_buffer_response *respuesta_memoria = recibir_buffer_response(fd_memoria);
    if (respuesta_memoria->resultado == OPERATION_FAILED)
        return NULL;

    void *datos_leidos = malloc(respuesta_memoria->tamanio_buffer);
    memcpy(datos_leidos, respuesta_memoria->buffer, respuesta_memoria->tamanio_buffer);
    destruir_buffer_response(respuesta_memoria);
    return datos_leidos;
}

void *recibir_contenido_pagina()
{
    t_mem_buffer_response *respuesta_memoria = recibir_buffer_response(fd_memoria);

    if (respuesta_memoria->resultado == OPERATION_FAILED)
        return NULL;

    void *contenido_pagina = malloc(get_tamanio_pagina());
    memcpy(contenido_pagina, respuesta_memoria->buffer, get_tamanio_pagina());

    destruir_buffer_response(respuesta_memoria);

    return contenido_pagina;
}

// TODO: Implementar get_cantidad_niveles, get_cantidad_entradas_tp y get_tamanio_pagina

u_int32_t get_cantidad_niveles()
{
    return 0;
}

u_int32_t get_cantidad_entradas_tp()
{
    return 0;
}

u_int32_t get_tamanio_pagina()
{
    return 0;
}

void enviar_peticion_instruccion(u_int32_t pid, u_int32_t program_counter)
{
    t_peticion_cpu *peticion_instruccion = crear_peticion_instruccion(pid, program_counter);
    enviar_peticion_cpu(fd_memoria, peticion_instruccion);
    destruir_peticion_cpu(peticion_instruccion);
}

void enviar_peticion_marco(u_int32_t pid, char *entradas_por_nivel)
{
    t_peticion_cpu *peticion_marco = crear_peticion_nro_marco(pid, entradas_por_nivel);
    enviar_peticion_cpu(fd_memoria, peticion_marco);
    destruir_peticion_cpu(peticion_marco);
}

void enviar_peticion_escritura(u_int32_t pid, u_int32_t direccion_fisica, void *contenido, u_int32_t tamanio_bytes)
{
    t_peticion_cpu *peticion_escritura = crear_peticion_escritura(pid, direccion_fisica, tamanio_bytes, contenido);
    enviar_peticion_cpu(fd_memoria, peticion_escritura);
    destruir_peticion_cpu(peticion_escritura);
}

void enviar_peticion_lectura(u_int32_t pid, u_int32_t direccion_fisica, u_int32_t tamanio_bytes)
{
    t_peticion_cpu *peticion_lectura = crear_peticion_lectura(pid, direccion_fisica, tamanio_bytes);
    enviar_peticion_cpu(fd_memoria, peticion_lectura);
    destruir_peticion_cpu(peticion_lectura);
}

void enviar_peticion_escritura_pagina(u_int32_t pid, u_int32_t direccion_fisica, void *contenido)
{
    t_peticion_cpu *peticion_contenido_pagina = crear_peticion_escritura(pid, direccion_fisica, get_tamanio_pagina(), contenido);
    enviar_peticion_cpu(fd_memoria, peticion_contenido_pagina);
    destruir_peticion_cpu(peticion_contenido_pagina);
}

void enviar_peticion_lectura_pagina(u_int32_t pid, u_int32_t direccion_fisica)
{
    t_peticion_cpu *peticion_contenido_pagina = crear_peticion_lectura(pid, direccion_fisica, get_tamanio_pagina());
    enviar_peticion_cpu(fd_memoria, peticion_contenido_pagina);
    destruir_peticion_cpu(peticion_contenido_pagina);
}