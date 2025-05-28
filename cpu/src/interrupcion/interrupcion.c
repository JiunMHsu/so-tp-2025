#include "interrupcion.h"

static int32_t fd_interrupt;

static int8_t interrupcion;
static pthread_mutex_t mutex_interrupcion;

void *atender_kernel_interrupt(void *_);

void inicializar_interrupcion(int32_t _fd_interrupt)
{
    fd_interrupt = _fd_interrupt;
    interrupcion = 0;
    pthread_mutex_init(&mutex_interrupcion, NULL);

    pthread_t hilo_interrupt;
    pthread_create(&hilo_interrupt, NULL, &atender_kernel_interrupt, NULL);
    pthread_detach(hilo_interrupt);
}

void *atender_kernel_interrupt(void *_)
{
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

        pthread_mutex_lock(&mutex_interrupcion);
        interrupcion = 1;
        pthread_mutex_unlock(&mutex_interrupcion);
    }

    return NULL;
}

int8_t hay_interrupcion()
{
    pthread_mutex_lock(&mutex_interrupcion);
    int8_t resultado = interrupcion;
    pthread_mutex_unlock(&mutex_interrupcion);

    return resultado;
}

void resetear_interrupcion()
{
    pthread_mutex_lock(&mutex_interrupcion);
    interrupcion = 0;
    pthread_mutex_unlock(&mutex_interrupcion);
}
