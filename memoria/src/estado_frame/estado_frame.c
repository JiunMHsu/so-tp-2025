#include "estado_frame.h"

static t_bitarray *bitmap;
static pthread_mutex_t bitmap_mutex;

static u_int32_t frames_totales;
static u_int32_t frames_libres;

static void liberar_frame(void *frame);

void inicializar_bitmap_estados()
{
    frames_totales = get_tam_memoria() / get_tam_pagina();
    frames_libres = frames_totales;

    u_int32_t tam_bitmap = (u_int32_t)ceil(frames_totales / 8.0);
    char *buffer = calloc(1, tam_bitmap); // incializado en 0 (libre)
    bitmap = bitarray_create_with_mode(buffer, tam_bitmap, LSB_FIRST);

    pthread_mutex_init(&bitmap_mutex, NULL);
}

t_list *ocupar_frames(u_int32_t cantidad_frames)
{
    pthread_mutex_lock(&bitmap_mutex);

    if (cantidad_frames > frames_libres)
    {
        pthread_mutex_unlock(&bitmap_mutex);
        log_mensaje_advertencia("No hay suficientes frames libres para ocupar");
        return NULL;
    }

    t_list *frames_ocupados = list_create();
    for (u_int32_t i = 0; i < frames_totales && cantidad_frames > 0; i++)
    {
        if (bitarray_test_bit(bitmap, i)) // si está ocupado (es 1, true)
            continue;

        bitarray_set_bit(bitmap, i);
        frames_libres--;

        u_int32_t *frame_ocupado = malloc(sizeof(u_int32_t));
        *frame_ocupado = i;
        list_add(frames_ocupados, frame_ocupado);
        cantidad_frames--;
    }

    pthread_mutex_unlock(&bitmap_mutex);
    return frames_ocupados;
}

void liberar_frames(t_list *frames)
{
    pthread_mutex_lock(&bitmap_mutex);
    list_iterate(frames, &liberar_frame);
    pthread_mutex_unlock(&bitmap_mutex);
}

u_int32_t get_cantidad_frames_disponibles()
{
    pthread_mutex_lock(&bitmap_mutex);
    u_int32_t cantidad_frames_disponibles = frames_libres;
    pthread_mutex_unlock(&bitmap_mutex);
    return cantidad_frames_disponibles;
}

void destruir_bitmap_estados()
{
    bitarray_destroy(bitmap);
    pthread_mutex_destroy(&bitmap_mutex);
}

static void liberar_frame(void *frame)
{
    u_int32_t _frame = *(u_int32_t *)frame;
    bitarray_clean_bit(bitmap, _frame);
    frames_libres++;
}
