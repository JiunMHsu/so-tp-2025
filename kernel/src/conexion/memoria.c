#include "memoria.h"

void *atender_memoria(void *fd_prt);

int32_t conectar_memoria(void)
{
    mem_address datos_memoria = get_mem_address();

    int32_t fd_conexion_memoria = crear_conexion(datos_memoria.ip, datos_memoria.puerto);

    int32_t respuesta_conexion_memoria = handshake(fd_conexion_memoria, KERNEL);

    if (respuesta_conexion_memoria == -1)
    {
        perror("No se puedo establecer la conexion con la memoria");
        cerrar_conexion(fd_conexion_memoria);
        return -1;
    }

    pthread_t hilo_memoria;
    pthread_create(&hilo_memoria, NULL, &atender_memoria, &fd_conexion_memoria);
    pthread_detach(hilo_memoria);

    return 0;
}

void *atender_memoria(void *fd_prt)
{
    int32_t fd_conexion_memoria = *((int *)fd_prt);

    while (1)
    {
        enviar_mensaje("Mensaje desde Kernel", fd_conexion_memoria);
        sleep(2);
    }

    return NULL;
}