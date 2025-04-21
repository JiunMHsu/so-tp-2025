#include <unistd.h>

#include <utils/sockets/sockets.h>
#include <utils/protocol/protocol.h>

#include "logger/logger.h"
#include "config/config.h"

typedef struct
{
    u_int32_t pid;
    u_int32_t tiempo;
} peticion_kernel;

int32_t fd_kernel;

int8_t conectar_con_kernel(char *nombre_interfaz);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Debes ingresarlo de la siguiente manera: %s <nombre_interfaz>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *nombre_interfaz = argv[1];

    iniciar_config();

    iniciar_logger(get_log_level());

    int8_t resultado = conectar_con_kernel(nombre_interfaz);
    if (resultado == -1)
        return EXIT_FAILURE;

    while (1)
    {
        char *mensaje = recibir_mensaje(fd_kernel);
        if (mensaje == NULL)
        {
            printf("El servidor se desconectó. Cerrando conexión...\n");
            cerrar_conexion(fd_kernel);
            return EXIT_FAILURE;
        }

        printf("Mensaje recibido: %s\n", mensaje);
        recibir_peticion(mensaje);
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

    return 0;
}

void recibir_peticion(char *mensaje)
{

    peticion_kernel peticion;

    if (sscanf(mensaje, "%d:%d", &peticion.pid, &peticion.tiempo) != 2)
    {
        log_error_peticion(mensaje);
        free(mensaje);

        return;
    }

    log_inicio_io(peticion.pid, peticion.tiempo);
    usleep(peticion.tiempo); // En que unidad manda el tiempo el kernel?
    log_finalizacion_io(peticion.pid);
    enviar_mensaje("Fin de IO. Se espera la siguiente peticion", fd_kernel);

    free(mensaje);
}