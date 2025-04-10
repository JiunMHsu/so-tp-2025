#include <utils/sockets/sockets.h>
#include <utils/protocol/protocol.h>

int32_t fd_kernel;

int8_t conectar_con_kernel(char *nombre_interfaz);

int main(int argc, char *argv[])
{
    int8_t resultado = conectar_con_kernel("IO_1");
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
        free(mensaje);
    }

    return EXIT_SUCCESS;
}

int8_t conectar_con_kernel(char *nombre_interfaz)
{
    fd_kernel = crear_conexion("127.0.0.1", "8003");
    int32_t response = handshake(fd_kernel, IO);

    if (response == -1)
    {
        cerrar_conexion(fd_kernel);
        return -1;
    }

    return 0;
}
