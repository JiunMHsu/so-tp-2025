#include "estado_frame.h"

static t_bitarray *bitmap;
static pthread_mutex_t bitmap_mutex;

static u_int32_t frames_totales;
static u_int32_t frames_libres;

void inicializar_bitmap_estados()
{
    frames_totales = get_tam_memoria() / get_tam_pagina();
    frames_libres = frames_totales;

    u_int32_t tam_bitmap = (u_int32_t)ceil(frames_totales / 8.0);
    char *buffer = calloc(1, tam_bitmap); // incializado en 0 (libre)
    bitmap = bitarray_create_with_mode(buffer, tam_bitmap, LSB_FIRST);

    pthread_mutex_init(&bitmap_mutex, NULL);
}

void set_estado_frame(u_int32_t frame, t_estado_frame estado)
{
    pthread_mutex_lock(&bitmap_mutex);

    if ((t_estado_frame)bitarray_test_bit(bitmap, frame) == estado)
    {
        pthread_mutex_unlock(&bitmap_mutex);
        return;
    }

    if (estado == LIBRE)
    {
        bitarray_clean_bit(bitmap, frame);
        frames_libres++;
    }
    else
    {
        bitarray_set_bit(bitmap, frame);
        frames_libres--;
    }

    pthread_mutex_unlock(&bitmap_mutex);
}

int32_t get_frame_libre()
{
    pthread_mutex_lock(&bitmap_mutex);

    for (int i = 0; i < frames_totales; i++)
    {
        if (bitarray_test_bit(bitmap, i)) // si está ocupado (es 1, true)
            continue;

        pthread_mutex_unlock(&bitmap_mutex);
        return i;
    }

    // Caso en el que todos los marcos esten ocupados
    pthread_mutex_unlock(&bitmap_mutex);
    log_mensaje_advertencia("No se encontraron frames libres");

    return -1;
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
