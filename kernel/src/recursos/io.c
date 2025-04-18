#include "io.h"

// TODO: reemplazar por colecciones thread-safe (listas y colas con mutex)
t_dictionary *ios;
t_queue *finalizados;

void inicializar_io()
{
    // TODO: inicializar estructura de datos para manejar IOs
}

void conectar_io(char *nombre, int32_t fd_io)
{
}

void *manejar_conexion_io(void *fd_ptr)
{

}
