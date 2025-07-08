#include "swap.h"

FILE *archivo_swap;
static t_dictionary *procesos_en_swap;
pthread_mutex_t mutex_swap = PTHREAD_MUTEX_INITIALIZER;

void inicializar_swap()
{
    archivo_swap = fopen(get_path_swapfile(), "wb+");

    if (archivo_swap == NULL)
        log_mensaje_error("No se pudo iniciar la swap.");

    pthread_mutex_init(&mutex_swap, NULL);
    procesos_en_swap = dictionary_create();
}

void cerrar_swap()
{
    fclose(archivo_swap);
    dictionary_destroy_and_destroy_elements(procesos_en_swap, free);
    pthread_mutex_destroy(&mutex_swap);
}

void guardar_en_swap(u_int32_t pid, u_int32_t cantidad_paginas, void *origen)
{
    pthread_mutex_lock(&mutex_swap);

    u_int32_t bytes_a_escribir = get_tam_pagina() * cantidad_paginas;

    fseek(archivo_swap, 0, SEEK_END);
    u_int32_t offset = ftell(archivo_swap);

    fseek(archivo_swap, offset, SEEK_SET);
    fwrite(origen, bytes_a_escribir, 1, archivo_swap);
    fflush(archivo_swap);

    t_swap_info *info = malloc(sizeof(t_swap_info));
    info->offset = offset;
    info->paginas = cantidad_paginas;
    dictionary_put(procesos_en_swap, string_itoa(pid), info);

    usleep(get_retardo_swap() * 1000);
    incrementar_swap_out(pid);

    pthread_mutex_unlock(&mutex_swap);
}

void recuperar_proceso_de_swap(u_int32_t pid, void *destino)
{
    pthread_mutex_lock(&mutex_swap);

    t_swap_info *info = dictionary_get(procesos_en_swap, string_itoa(pid));

    if (info == NULL)
    {
        log_mensaje_error("Se intento recuperar un proceso que no esta en swap.");
        pthread_mutex_unlock(&mutex_swap);
        return;
    }

    u_int32_t tam_pagina = get_tam_pagina();
    u_int32_t bytes_a_leer = tam_pagina * info->paginas;

    fseek(archivo_swap, info->offset, SEEK_SET);
    fread(destino, bytes_a_leer, 1, archivo_swap);

    usleep(get_retardo_swap() * 1000);
    incrementar_swap_in(pid);

    pthread_mutex_unlock(&mutex_swap);
}

// No eliminamos bytes físicos del archivo (queda fragmentado),
// pero sí liberamos el registro y podría reutilizarse en futuras mejoras.
void liberar_swap(u_int32_t pid)
{
    pthread_mutex_lock(&mutex_swap);

    t_swap_info *info = dictionary_remove(procesos_en_swap, string_itoa(pid));

    if (info != NULL)
        free(info);

    pthread_mutex_unlock(&mutex_swap);
}
