#include "logger/logger.h"
#include "servidor/servidor.h"

void escuchar_conexiones(void *);
void *atender_cliente(void *);

int main(int argc, char *argv[])
{
    iniciar_config();
    iniciar_logger(get_log_level());

    int32_t fd_memoria = iniciar_servidor();
    escuchar_conexiones(&fd_memoria);

    finalizar_servidor();

    return 0;
}

// void * se usa porque cuando crees el hilo lo va a necesitar asi
void escuchar_conexiones(void *fd_escucha)
{
    while (1)
        esperar_cliente(*((int32_t *)fd_escucha), &atender_cliente);
}

void *atender_cliente(void *fd_ptr)
{
    int32_t fd_conexion = *((int32_t *)fd_ptr);

    uint32_t modulo_cliente = recibir_cliente(fd_conexion);

    switch (modulo_cliente)
    {
    case KERNEL:
        while (1)
        {
            char *mensaje = recibir_mensaje(fd_conexion);
            if (mensaje == NULL)
            {
                perror("Cliente desconectado.\n");
                return NULL;
            }
            printf("Mensaje recibido de Kernel: %s\n", mensaje);
            free(mensaje);
        }
        break;

    case CPU:
        while (1)
        {
            char *mensaje = recibir_mensaje(fd_conexion);
            if (mensaje == NULL)
            {
                perror("Cliente desconectado.\n");
                return NULL;
            }
            printf("Mensaje recibido de CPU: %s\n", mensaje);
            free(mensaje);
        }
        break;

    default:
        log_mensaje_error("Error cliente inválido");
        close(fd_conexion);
        return NULL;
        break;
    }
    return NULL;
}