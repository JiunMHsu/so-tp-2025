#include "memoria.h"

int32_t conectar_memoria(void)
{
    mem_address datos_memoria = get_memoria_address();

    int32_t fd_conexion_memoria = crear_conexion(datos_memoria.ip, datos_memoria.puerto);

    int32_t respuesta_conexion_memoria = handshake(fd_conexion_memoria, CPU);

    if (respuesta_conexion_memoria == -1)
    {
        perror("No se puedo establecer la conexion con la memoria");
        cerrar_conexion(fd_conexion_memoria);
        return -1;
    }

    return fd_conexion_memoria;
}

void *atender_memoria(void *fd_prt)
{
    int32_t fd_conexion_memoria = *((int *)fd_prt);

    while (1)
    {
        enviar_mensaje("Mensaje desde CPU", fd_conexion_memoria);
        sleep(2);
    }

    return NULL;
}