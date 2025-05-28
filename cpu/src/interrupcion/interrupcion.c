#include "interrupcion.h"

int8_t llego_interrupcion;


// otras globales de interrupcion

void *atender_kernel_interrupt(void *_);

void inicializar_interrupcion(int32_t fd_interrupt)
{
    llego_interrupcion = 0;

    pthread_t hilo_interrupt;

    pthread_create(&hilo_interrupt, NULL, &atender_kernel_interrupt, fd_interrupt);
    pthread_detach(hilo_interrupt);
}

void *atender_kernel_interrupt(void *fd_ptr)
{
    // escuchar de kernel
    // si le cae un int, setear el global

    // TODO: el acceso a la variable global tiene que ser atomico, tendria que haber un mutex => cuando llamo a hay_interrupcion en otro hilo, podria estar modificando el valor aca
    int32_t fd_interrupt = *((int32_t *)fd_ptr);
    free(fd_ptr);

    while (1)
    {
        int32_t senial_interrupcion = recibir_senial(fd_interrupt);

        if (senial_interrupcion == -1)
        {
            log_mensaje_error("Error al recibir señal de interrupcion. Cerrando conexión con kernel...");
            cerrar_conexion(fd_interrupt);
            return NULL;
        }

        log_interrupcion_recibida();

        // TODO: falta mutex
        llego_interrupcion = 1;
    }

    return NULL;
}

int8_t hay_interrupcion()
{
    // TODO: falta mutex??
    return llego_interrupcion;
}

void resetear_interrupcion()
{
    // TODO: falta mutex
    llego_interrupcion = 0;
}
