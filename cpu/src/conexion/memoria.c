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

void cerrar_conexion_memoria()
{
    log_mensaje_error("Cerrando conexión con memoria...");
    cerrar_conexion(fd_memoria);
}

char *recibir_mensaje_memoria()
{
    return recibir_mensaje(fd_memoria);
}

u_int32_t recibir_marco()
{
    return recibir_senial(fd_memoria);
}

void enviar_peticion_instruccion(u_int32_t pid, u_int32_t program_counter)
{
    t_peticion_cpu *peticion_instruccion = crear_peticion_instruccion(pid, program_counter);
    enviar_peticion_cpu(fd_memoria, peticion_instruccion);
    destruir_peticion_cpu(peticion_instruccion);
}
