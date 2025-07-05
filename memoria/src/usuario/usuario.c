#include "usuario.h"

void *memoria_usuario;
pthread_mutex_t memoria_usuario_mutex;

void inicializar_memoria_usuario()
{
    memoria_usuario = malloc(get_tam_memoria());
    inicializar_bitmap_estados();
    pthread_mutex_init(&memoria_usuario_mutex, NULL);
}

void destruir_memoria_usuario()
{
    free(memoria_usuario);
    destruir_bitmap_estados();
}
