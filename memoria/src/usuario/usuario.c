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

// Las siguientes funciones son auxiliares para manejar la memoria de usuario.
// No se si corresponden hacer log de "acceso_espacio_usuario" ya que no son accesos directos de un proceso,
// sino que son funciones internas de la memoria.
// Sirven para hacer memory dumps y swapping

void *leer_pagina_por_marco(u_int32_t marco)
{
    u_int32_t _dir = marco * get_tam_pagina();
    void *pagina = malloc(get_tam_pagina());

    pthread_mutex_lock(&memoria_usuario_mutex);
    memcpy(pagina, memoria_usuario + _dir, get_tam_pagina());
    pthread_mutex_unlock(&memoria_usuario_mutex);

    return pagina;
}

void escribir_marco_entero(u_int32_t marco, void *contenido)
{
    u_int32_t _dir = marco * get_tam_pagina();

    pthread_mutex_lock(&memoria_usuario_mutex);
    memcpy(memoria_usuario + _dir, contenido, get_tam_pagina());
    pthread_mutex_unlock(&memoria_usuario_mutex);
}

t_list *leer_paginas_por_marcos(t_list *marcos)
{
    t_list *paginas = list_create();

    t_list_iterator *iterador_marcos = list_iterator_create(marcos);
    while (list_iterator_has_next(iterador_marcos))
    {
        u_int32_t marco = *(u_int32_t *)list_iterator_next(iterador_marcos);
        void *pagina = leer_pagina_por_marco(marco);
        list_add(paginas, pagina);
    }
    list_iterator_destroy(iterador_marcos);

    return paginas;
}
