#include "memoria.h"

static int32_t conectar_memoria(void);
static void desconectar_memoria(int32_t fd_memoria);
static int8_t enviar_solicitud(t_kernel_mem_req *mem_request);

static int32_t conectar_memoria()
{
    mem_address datos_memoria = get_mem_address();
    int32_t fd_memoria = crear_conexion(datos_memoria.ip, datos_memoria.puerto);

    if (handshake(fd_memoria, KERNEL) == -1)
    {
        log_mensaje_error("Error al conectar a memoria");
        cerrar_conexion(fd_memoria);
        exit(EXIT_FAILURE);
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

static int8_t enviar_solicitud(t_kernel_mem_req *mem_request)
{
    int32_t fd_memoria = conectar_memoria();

    enviar_kernel_mem_request(fd_memoria, mem_request);
    destruir_kernel_mem_request(mem_request);

    return recibir_senial(fd_memoria);
    if (respuesta == -1)
    {
        log_mensaje_error("Error al recibir respuesta de memoria");
        desconectar_memoria(fd_memoria);
        exit(EXIT_FAILURE);
    }

    desconectar_memoria(fd_memoria);
    return respuesta;
}

int8_t solicitar_creacion_proceso(u_int32_t pid, u_int32_t tamanio, char *ruta_codigo)
{
    t_kernel_mem_req *mem_request = crear_kernel_mem_request(INICIAR_PROCESO, pid, ruta_codigo);
    return enviar_solicitud(mem_request);
}

int8_t solicitar_finalizacion_proceso(u_int32_t pid)
{
    t_kernel_mem_req *mem_request = crear_kernel_mem_request(FINALIZAR_PROCESO, pid, NULL);
    return enviar_solicitud(mem_request);
}

int8_t solicitar_dump_proceso(u_int32_t pid)
{
    t_kernel_mem_req *mem_request = crear_kernel_mem_request(DUMP_PROCESO, pid, NULL);
    return enviar_solicitud(mem_request);
}

int8_t solicitar_swap_out(u_int32_t pid)
{
    t_kernel_mem_req *mem_request = crear_kernel_mem_request(SWAP_OUT, pid, NULL);
    return enviar_solicitud(mem_request);
}

int8_t solicitar_swap_in(u_int32_t pid)
{
    t_kernel_mem_req *mem_request = crear_kernel_mem_request(SWAP_IN, pid, NULL);
    return enviar_solicitud(mem_request);
}
