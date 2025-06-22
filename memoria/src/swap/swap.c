#include "swap.h"

FILE *archivo_swap;

void inicializar_swap()
{
    archivo_swap = fopen(get_path_swapfile(), "wb+");

    if (archivo_swap == NULL)
        log_mensaje_error("No se pudo iniciar la swap.");
}

void cerrar_swap()
{
    fclose(archivo_swap);
}