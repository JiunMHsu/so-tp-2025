#include "swap.h"

static t_dictionary *swapped;
static pthread_mutex_t mutex_swap;

static char *path_swapfile;

static t_swapped *crear_swapped(u_int32_t paginas, u_int32_t posicion);

void inicializar_swap()
{
    path_swapfile = get_path_swapfile();
    FILE *swap = fopen(path_swapfile, "w+b");

    if (archivo_swap == NULL)
    {
        log_mensaje_error("No se pudo iniciar el swap.");
        exit(EXIT_FAILURE);
    }

    fclose(swap);
    swapped = dictionary_create();
    pthread_mutex_init(&mutex_swap, NULL);
}

void guardar_en_swap(u_int32_t pid, t_list *paginas)
{
    u_int32_t tam_pagina = get_tam_pagina();
    u_int32_t posicion_escritura = 0;
    u_int32_t cantidad_paginas = list_size(paginas);

    char *_pid = string_itoa(pid);

    pthread_mutex_lock(&mutex_swap);

    FILE *archivo_swap = fopen(path_swapfile, "r+b");

    if (!dictionary_has_key(_pid)) // primera vez que se guarda un proceso en swap
    {
        fseek(archivo_swap, 0, SEEK_END);
        u_int32_t posicion = ftell(archivo_swap) / tam_pagina;
        t_swapped *nuevo_swapped = crear_swapped(cantidad_paginas, posicion);
        dictionary_put(swapped, strdup(_pid), nuevo_swapped);
    }

    t_swapped *proceso = dictionary_get(swapped, _pid);
    if (cantidad_paginas != proceso->paginas)
    {
        log_mensaje_error("La cantidad de páginas cambió.");
        exit(EXIT_FAILURE); // paniquea porque no debería ocurrir
    }

    fseek(archivo_swap, proceso->posicion * tam_pagina, SEEK_SET);
    t_list_iterator *iterador_paginas = list_iterator_create(paginas);

    while (list_iterator_has_next(iterador_paginas))
    {
        void *pagina = list_iterator_next(iterador_paginas);
        fwrite(pagina, tam_pagina, 1, archivo_swap);
    }

    list_iterator_destroy(iterador_paginas);

    fflush(archivo_swap); // necesario?
    fclose(archivo_swap);

    pthread_mutex_unlock(&mutex_swap);

    free(_pid);
    usleep(get_retardo_swap() * 1000);
    incrementar_swap_out(pid);
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

static t_swapped *crear_swapped(u_int32_t paginas, u_int32_t posicion)
{
    t_swapped *swapped = malloc(sizeof(t_swapped));
    swapped->paginas = paginas;
    swapped->posicion = posicion;

    return swapped;
}