#include <utils/sockets/sockets.h>
#include <utils/protocol/protocol.h>
#include "logger/logger.h"
#include "config/config.h"

int32_t fd_kernel;

int8_t conectar_con_kernel(char *nombre_interfaz);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Debes ingresarlo de la siguiente manera: %s <nombre_interfaz>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *nombre_interfaz = argv[1];

    iniciar_config();

    iniciar_logger(get_log_level());

    /* Loggeo el valor de config para confirmar los valores
    log_info(logger, "IP_KERNELL: %s", IP_KERNELL);
    log_info(logger, "PUERTO_KERNELL: %d", PUERTO_KERNELL);
    log_info(logger, "LOG_LEVEL: %s", LOG_LEVEL_STR); */

    // Creo conexion hacia kernell (aca quede)
    int8_t resultado = conectar_con_kernel(nombre_interfaz);
    if (resultado == -1)
        return EXIT_FAILURE;

    while (1) // Es una espera activa? Hay que hacer uso del semaforo?
    {
        char *mensaje = recibir_mensaje(fd_kernel);
        if (mensaje == NULL)
        {
            printf("El servidor se desconectó. Cerrando conexión...\n");
            cerrar_conexion(fd_kernel);
            return EXIT_FAILURE;
        }

        printf("Mensaje recibido: %s\n", mensaje);
        free(mensaje);
    }

    return EXIT_SUCCESS;
}

int8_t conectar_con_kernel(char *nombre_interfaz)
{
    kernel_address address = get_kernel_address();
    fd_kernel = crear_conexion(address.ip, address.puerto);
    int32_t response = handshake(fd_kernel, IO);

    if (response == -1)
    {
        cerrar_conexion(fd_kernel);
        return -1;
    }
    enviar_mensaje(nombre_interfaz, fd_kernel);

    return 0;
}
