#include "interrupcion.h"

int32_t fd_interrupt;

// otras globales de interrupcion

void *atender_kernel_interrupt(void *_);

void inicializar_interrupcion(int32_t fd_interrupt)
{
    pthread_t hilo_interrupt;

    pthread_create(&hilo_interrupt, NULL, &atender_kernel_interrupt, NULL);
    pthread_detach(hilo_interrupt);
}

// TODO: Implementar la logica de interrupcion
void *atender_kernel_interrupt(void *_)
{
    // escuchar de kernel
    // si le cae un int, setear el global
    return NULL;
}

// TODO: Implementar la logica de interrupcion
int8_t hay_interrupcion()
{
    return 0;
}

// TODO: Implementar la logica de interrupcion
void resetear_interrupcion() {}
