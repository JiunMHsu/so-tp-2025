#include "memoria.h"

static int32_t conectar_memoria(void);
static void desconectar_memoria(int32_t fd_memoria);

static int32_t conectar_memoria()
{
    mem_address datos_memoria = get_mem_address();
    int32_t fd_memoria = crear_conexion(datos_memoria.ip, datos_memoria.puerto);

    if (handshake(fd_memoria, KERNEL) == -1)
    {
        log_mensaje_error("Error al conectar a memoria");
        cerrar_conexion(fd_memoria);
        return -1;
    }

    return fd_memoria;
}

static void desconectar_memoria(int32_t fd_memoria)
{
    if (fd_memoria != -1)
    {
        cerrar_conexion(fd_memoria);
        log_evento("Memoria desconectada");
    }
}

// TODO: implementar
int32_t solicitar_creacion_proceso(u_int32_t pid, u_int32_t tamanio, char *ruta_codigo)
{
    // crear una mem_request (definida en utils)
    // operación: INICIAR_PROCESO
    // { operacion, pid, tamanio, ruta_codigo } (definida en utils)

    int32_t fd_memoria = conectar_memoria();
    // enviar la mem_request (definida en utils)
    // destruir la mem_request (definida en utils)

    // recibir mem_response (definida en utils)
    desconectar_memoria(fd_memoria);

    // retornar la respuesta
    return -1;
}

// TODO: implementar
int32_t solicitar_finalizacion_proceso(u_int32_t pid)
{
    // crear una mem_request (definida en utils)
    // operación: FINALIZAR_PROCESO
    // { operacion, pid } (definida en utils)

    // conectar a memoria
    int32_t fd_memoria = conectar_memoria();
    // enviar la mem_request (definida en utils)
    // destruir la mem_request (definida en utils)

    // recibir mem_response (definida en utils)
    desconectar_memoria(fd_memoria);

    // retornar la respuesta
    return -1;
}
