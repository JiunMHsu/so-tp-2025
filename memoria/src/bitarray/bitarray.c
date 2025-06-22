#include "bitarray.h"

static u_int32_t frames_totales;
static t_bitarray *bitmap;
static pthread_mutex_t bitmap_mutex;

void incializar_bitmap()
{
    frames_totales = get_tam_memoria() / get_tam_pagina();
    if (frames_totales < 0)
    {
        log_mensaje_error("Cantidad invalida de frames");
        exit(EXIT_FAILURE);
    }

    u_int32_t tam_bitmap = ceil(frames_totales / 8.0);
    char *buffer = malloc(tam_bitmap);
    if (buffer == NULL)
    {
        log_mensaje_error("No se pudo crear memoria para bitmap");
        exit(EXIT_FAILURE);
    }

    memset(buffer, 0, tam_bitmap);
    bitmap = bitarray_create_with_mode(buffer, tam_bitmap, LSB_FIRST);
    pthread_mutex_init(&bitmap_mutex, NULL);
}

int32_t asignar_marco()
{
    pthread_mutex_lock(&bitmap_mutex);

    for (u_int32_t i = 0; i < frames_totales; i++)
    {
        if (bitarray_test_bit(bitmap, i) != 0)
            continue;

        bitarray_set_bit(bitmap, i);
        pthread_mutex_unlock(&bitmap_mutex);
        return i;
    }

    // Caso en el que todos los marcos esten ocupados
    pthread_mutex_unlock(&bitmap_mutex);
    log_mensaje_advertencia("No se encontraron frames libres");

    return -1;
}

void liberar_marco(u_int32_t frame_a_liberar)
{
    if (frame_a_liberar < 0 || frame_a_liberar >= frames_totales)
    {
        log_mensaje_error("El frame solicitado se encuentra fuera de rango");
        exit(EXIT_FAILURE);
    }

    pthread_mutex_lock(&bitmap_mutex);

    if (bitarray_test_bit(bitmap, frame_a_liberar) == 0)
    {
        pthread_mutex_unlock(&bitmap_mutex);
        log_mensaje_advertencia("El frame solicitado ya se encontraba liberado");
    }

    bitarray_clean_bit(bitmap, frame_a_liberar);
    pthread_mutex_unlock(&bitmap_mutex);
}

void destruir_bitmap()
{
    bitarray_destroy(bitmap);
    pthread_mutex_destroy(&bitmap_mutex);
}
