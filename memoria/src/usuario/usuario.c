#include "usuario.h"

void *memoria_usuario;

void inicializar_memoria_usuario()
{
    memoria_usuario = malloc(get_tam_memoria());
    if(memoria_usuario == NULL)
    {
        log_mensaje_error("No se pudo iniciar la memoria");
    }

    incializar_bitmap();
}

void destruir_memoria_usuario()
{
    free(memoria_usuario);
    destruir_bitmap();
}