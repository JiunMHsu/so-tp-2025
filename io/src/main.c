#include <utils/sockets/sockets.h>
#include <utils/protocol/protocol.h>

int8_t conectar_con_kernel(char *nombre_interfaz);

int main(int argc, char *argv[])
{
    int8_t resultado = conectar_con_kernel("IO_1");
    if (resultado == -1)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

int8_t conectar_con_kernel(char *nombre_interfaz)
{
    int32_t fd_kernel = crear_conexion("127.0.0.1", "8001");
    int32_t response = handshake(fd_kernel, IO);
    if (response == -1)
    {
        liberar_conexion(fd_kernel);
        return -1;
    }

    enviar_mensaje(nombre_interfaz, fd_kernel);

    return 0;
}