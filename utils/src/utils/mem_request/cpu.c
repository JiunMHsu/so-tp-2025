#include "cpu.h"

static t_peticion_cpu *crear_peticion_cpu(operacion_cpu_memoria, u_int32_t, u_int32_t, char *, char *, u_int32_t, u_int32_t, void *);

static t_peticion_cpu *crear_peticion_cpu(operacion_cpu_memoria operacion,
                                          u_int32_t pid,
                                          u_int32_t program_counter,
                                          char *entradas_por_nivel,
                                          char *direccion_fisica,
                                          u_int32_t frame,
                                          u_int32_t tamanio_buffer,
                                          void *buffer)
{
    t_peticion_cpu *peticion = malloc(sizeof(t_peticion_cpu));
    peticion->operacion = operacion;
    peticion->pid = pid;
    peticion->entradas_por_nivel = NULL;
    peticion->direccion_fisica = NULL;
    peticion->buffer = NULL;

    switch (operacion)
    {
    case FETCH_INSTRUCCION:
        peticion->program_counter = program_counter;
        break;
    case OBTENER_MARCO:
        peticion->entradas_por_nivel = strdup(entradas_por_nivel);
        break;
    case LEER:
        peticion->direccion_fisica = strdup(direccion_fisica);
        peticion->tamanio_buffer = tamanio_buffer;
        break;
    case ESCRIBIR:
        peticion->direccion_fisica = strdup(direccion_fisica);
        peticion->tamanio_buffer = tamanio_buffer;
        peticion->buffer = malloc(tamanio_buffer);
        memcpy(peticion->buffer, buffer, tamanio_buffer);
        break;
    case LEER_PAG:
        peticion->frame = frame;
        peticion->tamanio_buffer = tamanio_buffer;
        break;
    case ESCRIBIR_PAG:
        peticion->frame = frame;
        peticion->tamanio_buffer = tamanio_buffer;
        memcpy(peticion->buffer, buffer, tamanio_buffer);
        break;
    }

    return peticion;
}

t_peticion_cpu *crear_peticion_instruccion(u_int32_t pid, u_int32_t program_counter)
{
    return crear_peticion_cpu(FETCH_INSTRUCCION, pid, program_counter, NULL, NULL, 0, 0, NULL);
}

t_peticion_cpu *crear_peticion_nro_marco(u_int32_t pid, char *entradas_por_nivel)
{
    return crear_peticion_cpu(OBTENER_MARCO, pid, 0, entradas_por_nivel, NULL, 0, 0, NULL);
}

t_peticion_cpu *crear_peticion_lectura(u_int32_t pid, char *direccion_fisica, u_int32_t tamanio_buffer)
{
    return crear_peticion_cpu(LEER, pid, 0, NULL, direccion_fisica, 0, tamanio_buffer, NULL);
}

t_peticion_cpu *crear_peticion_escritura(u_int32_t pid, char *direccion_fisica, u_int32_t tamanio_buffer, void *buffer)
{
    return crear_peticion_cpu(ESCRIBIR, pid, 0, NULL, direccion_fisica, 0, tamanio_buffer, buffer);
}

t_peticion_cpu *crear_peticion_escritura_pagina(u_int32_t pid, u_int32_t frame, u_int32_t tamanio_pagina, void *contenido_pagina)
{
    return crear_peticion_cpu(ESCRIBIR_PAG, pid, 0, NULL, NULL, frame, tamanio_pagina, contenido_pagina);
}

t_peticion_cpu *crear_peticion_lectura_pagina(u_int32_t pid, u_int32_t frame, u_int32_t tamanio_pagina)
{
    return crear_peticion_cpu(ESCRIBIR_PAG, pid, 0, NULL, NULL, frame, tamanio_pagina, NULL);
}

void enviar_peticion_cpu(int32_t fd_memoria, t_peticion_cpu *peticion)
{
    t_packet *paquete = crear_paquete();

    agregar_a_paquete(paquete, &(peticion->operacion), sizeof(operacion_cpu_memoria));
    agregar_a_paquete(paquete, &(peticion->pid), sizeof(u_int32_t));

    switch (peticion->operacion)
    {
    case FETCH_INSTRUCCION:
        agregar_a_paquete(paquete, &(peticion->program_counter), sizeof(u_int32_t));
        break;
    case OBTENER_MARCO:
        agregar_a_paquete(paquete, peticion->entradas_por_nivel, strlen(peticion->entradas_por_nivel) + 1);
        break;
    case LEER:
        agregar_a_paquete(paquete, peticion->direccion_fisica, strlen(peticion->direccion_fisica) + 1);
        agregar_a_paquete(paquete, &(peticion->tamanio_buffer), sizeof(u_int32_t));
        break;
    case ESCRIBIR:
        agregar_a_paquete(paquete, peticion->direccion_fisica, strlen(peticion->direccion_fisica) + 1);
        agregar_a_paquete(paquete, &(peticion->tamanio_buffer), sizeof(u_int32_t));
        agregar_a_paquete(paquete, peticion->buffer, peticion->tamanio_buffer);
        break;
    case LEER_PAG:
        agregar_a_paquete(paquete, &(peticion->frame), sizeof(u_int32_t));
        agregar_a_paquete(paquete, &(peticion->tamanio_buffer), sizeof(u_int32_t));
        break;
    case ESCRIBIR_PAG:
        agregar_a_paquete(paquete, &(peticion->frame), sizeof(u_int32_t));
        agregar_a_paquete(paquete, &(peticion->tamanio_buffer), sizeof(u_int32_t));
        agregar_a_paquete(paquete, peticion->buffer, peticion->tamanio_buffer);
        break;
    }

    enviar_paquete(paquete, fd_memoria);
    eliminar_paquete(paquete);
}

t_peticion_cpu *recibir_peticion_cpu(int32_t fd_conexion)
{
    t_list *paquete = recibir_paquete(fd_conexion);
    if (paquete == NULL)
        return NULL;

    t_peticion_cpu *peticion = malloc(sizeof(t_peticion_cpu));
    peticion->operacion = *(operacion_cpu_memoria *)list_get(paquete, 0);
    peticion->pid = *(u_int32_t *)list_get(paquete, 1);

    switch (peticion->operacion)
    {
    case FETCH_INSTRUCCION:
        peticion->program_counter = *(u_int32_t *)list_get(paquete, 2);
        break;
    case OBTENER_MARCO:
        peticion->entradas_por_nivel = strdup(list_get(paquete, 2));
        break;
    case LEER:
        peticion->direccion_fisica = strdup(list_get(paquete, 2));
        peticion->tamanio_buffer = *(u_int32_t *)list_get(paquete, 3);
        break;
    case ESCRIBIR:
        peticion->direccion_fisica = strdup(list_get(paquete, 2));
        peticion->tamanio_buffer = *(u_int32_t *)list_get(paquete, 3);
        peticion->buffer = malloc(peticion->tamanio_buffer);
        memcpy(peticion->buffer, list_get(paquete, 4), peticion->tamanio_buffer);
        break;
    case LEER_PAG:
        peticion->frame = *(u_int32_t *)list_get(paquete, 2);
        peticion->tamanio_buffer = *(u_int32_t *)list_get(paquete, 3);
        break;
    case ESCRIBIR_PAG:
        peticion->frame = *(u_int32_t *)list_get(paquete, 2);
        peticion->tamanio_buffer = *(u_int32_t *)list_get(paquete, 3);
        peticion->buffer = malloc(peticion->tamanio_buffer);
        memcpy(peticion->buffer, list_get(paquete, 4), peticion->tamanio_buffer);
        break;
    }

    list_destroy_and_destroy_elements(paquete, &free);
    return peticion;
}

void destruir_peticion_cpu(t_peticion_cpu *peticion)
{
    if (peticion == NULL)
        return;

    switch (peticion->operacion)
    {
    case OBTENER_MARCO:
        free(peticion->entradas_por_nivel);
        break;
    case LEER:
        free(peticion->direccion_fisica);
        break;
    case ESCRIBIR:
        free(peticion->direccion_fisica);
        free(peticion->buffer);
        break;
    case ESCRIBIR_PAG:
        free(peticion->buffer);
        break;
    default: // cualquier otro caso
        break;
    }

    free(peticion);
    peticion = NULL;
}

t_list *convertir_a_lista_de_direcciones_fisicas(char *direcciones_fisicas)
{
    t_list *lista_direcciones = list_create();
    char **direcciones = string_split(direcciones_fisicas, " ");
    for (int i = 0; direcciones[i] != NULL; i++)
        list_add(lista_direcciones, direcciones[i]);

    free(direcciones);
    return lista_direcciones;
}
