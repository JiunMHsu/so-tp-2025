#include "usuario.h"

static void *memoria_usuario;
static pthread_mutex_t memoria_usuario_mutex;

static u_int32_t get_numero_de_frame(u_int32_t direccion_fisica);

void inicializar_espacio_usuario()
{
    memoria_usuario = malloc(get_tam_memoria());
    pthread_mutex_init(&memoria_usuario_mutex, NULL);
}

u_int8_t escribir_memoria_usuario(u_int32_t pid, u_int32_t direccion_fisica, void *buffer, u_int32_t tamanio_buffer)
{
    u_int32_t tamanio_guardado = 0;
    u_int32_t frame = get_numero_de_frame(direccion_fisica);
    u_int32_t limite_de_frame = frame * get_tam_pagina() + get_tam_pagina();
    u_int32_t _dir_fisica = direccion_fisica;

    for (int i = 0; tamanio_guardado < tamanio_buffer && _dir_fisica < limite_de_frame; i++)
    {
        pthread_mutex_lock(&memoria_usuario_mutex);
        memcpy(memoria_usuario + _dir_fisica, buffer, 1); // Va copiando byte por byte del buffer a la memoria
        pthread_mutex_unlock(&memoria_usuario_mutex);

        _dir_fisica++;
        buffer++;
        tamanio_guardado++;
    }

    if (tamanio_guardado == tamanio_buffer)
    {
        log_acceso_espacio_usuario(pid, ESCRITURA, direccion_fisica, tamanio_buffer);
        return 1;
    }

    return 0;
}

void *leer_memoria_usuario(u_int32_t pid, u_int32_t direccion_fisica, u_int32_t tamanio_buffer)
{
    void *buffer = calloc(tamanio_buffer, sizeof(char));
    u_int32_t tamanio_leido = 0;
    u_int32_t offset = 0;

    u_int32_t frame = get_numero_de_frame(direccion_fisica);
    u_int32_t limite_de_frame = frame * get_tam_pagina() + get_tam_pagina();
    u_int32_t _dir_fisica = direccion_fisica;

    printf("[+] Empty Buffer: %s\n", (char *)buffer);

    for (int i = 0; tamanio_leido < tamanio_buffer && _dir_fisica < limite_de_frame; i++)
    {
        pthread_mutex_lock(&memoria_usuario_mutex);
        memcpy(buffer + offset, memoria_usuario + _dir_fisica, sizeof(char)); // Va copiando byte por byte del buffer a la memoria
        pthread_mutex_unlock(&memoria_usuario_mutex);

        _dir_fisica++;
        offset++;
        tamanio_leido++;

        printf("[+] Buffer: %s\n", (char *)buffer);
    }

    printf("[+] Loaded Buffer: %s\n", (char *)buffer);
    if (tamanio_leido == tamanio_buffer)
    {
        log_acceso_espacio_usuario(pid, LECTURA, direccion_fisica, tamanio_buffer);
        return buffer;
    }

    free(buffer);
    return NULL;
}

static u_int32_t get_numero_de_frame(u_int32_t direccion_fisica)
{
    return direccion_fisica / get_tam_pagina();
}
