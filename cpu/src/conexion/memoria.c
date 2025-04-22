#include "memoria.h"

int32_t fd_memoria;

int8_t conectar_memoria()
{
    mem_address datos_memoria = get_memoria_address();
    fd_memoria = crear_conexion(datos_memoria.ip, datos_memoria.puerto);
    int32_t respuesta_conexion_memoria = handshake(fd_memoria, CPU);

    if (respuesta_conexion_memoria == -1)
    {
        log_mensaje_error("No se puedo establecer la conexion con la memoria");
        cerrar_conexion(fd_memoria);
        return -1;
    }

    return 0;
}
