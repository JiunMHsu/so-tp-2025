#include "usuario.h"

static void *memoria_usuario;
static pthread_mutex_t memoria_usuario_mutex;

void inicializar_espacio_usuario()
{
    memoria_usuario = malloc(get_tam_memoria());
    pthread_mutex_init(&memoria_usuario_mutex, NULL);
}
