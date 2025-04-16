
#ifndef UTILS_SOCKETS_H
#define UTILS_SOCKETS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <pthread.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>

typedef enum
{
    KERNEL,
    CPU,
    MEMORIA,
    IO,
} t_cliente;

int32_t crear_servidor(char *puerto);
int8_t esperar_cliente(int32_t fd_escucha, void *(*atender_cliente)(void *));

/**
 * @brief Recibe y responde al handshake del cliente que intenta conectarse.
 *
 * @param fd_conexion
 * @param cliente_esperado
 * @return int8_t 0 si el cliente es el esperado, -1 en caso contrario.
 */
int8_t recibir_cliente(int32_t fd_conexion);
int32_t crear_conexion(char *ip, char *puerto);
int32_t handshake(int32_t fd_conexion, t_cliente id_cliente);
void cerrar_conexion(int32_t socket_cliente);

#endif // UTILS_SOCKETS_H
