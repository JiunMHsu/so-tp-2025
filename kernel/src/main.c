#include <utils/sockets/sockets.h>
#include <utils/protocol/protocol.h>

void *escuchar_conexiones(void *fd_escucha);
void *atender_io(void *fd_ptr);
void finalizar_servidor(int32_t fd_escucha);

int main(int argc, char *argv[])
{
    int32_t fd_escucha = crear_servidor("8001");

    // pthread_t hilo_escucha;
    // pthread_create(&hilo_escucha, NULL, &escuchar_conexiones, &fd_escucha);
    // pthread_detach(hilo_escucha);

    escuchar_conexiones(&fd_escucha);

    finalizar_servidor(fd_escucha);
    return EXIT_SUCCESS;
}

void *escuchar_conexiones(void *fd_escucha)
{
    while (1)
        esperar_cliente(*((int32_t *)fd_escucha), &atender_io); // bloqueante

    return NULL;
}

void *atender_io(void *fd_ptr)
{
    int32_t fd_conexion = *((int32_t *)fd_ptr);
    // free(fd_ptr);

    // recibir el handshake
    uint32_t modulo_cliente = recibir_cliente(fd_conexion);

    if (modulo_cliente != IO)
    {
        printf("Error de Cliente \n");
        return NULL;
    }

    while (1)
    {
        char *mensaje = recibir_mensaje(fd_conexion);
        printf("Mensaje recibido: %s\n", mensaje);
    }
}

void finalizar_servidor(int32_t fd_escucha)
{
    close(fd_escucha);
}
