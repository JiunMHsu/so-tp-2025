#include "sockets.h"

int32_t crear_servidor(char *puerto)
{
    int32_t socket_servidor;

    struct addrinfo hints, *servinfo;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(NULL, puerto, &hints, &servinfo);

    socket_servidor = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    bind(socket_servidor, servinfo->ai_addr, servinfo->ai_addrlen);
    listen(socket_servidor, SOMAXCONN);

    freeaddrinfo(servinfo);

    return socket_servidor;
}

int8_t esperar_cliente(int32_t fd_escucha, void *(*atender_cliente)(void *))
{
    pthread_t thread;

    int32_t *fd_conexion_ptr = malloc(sizeof(int32_t));
    *fd_conexion_ptr = accept(fd_escucha, NULL, NULL);

    pthread_create(&thread, NULL, atender_cliente, fd_conexion_ptr);
    pthread_detach(thread);

    return 0;
}

int8_t recibir_cliente(int32_t fd_conexion, t_cliente cliente_esperado)
{
    int32_t id_cliente;
    int32_t resultOk = 0;
    int32_t resultError = -1;

    recv(fd_conexion, &id_cliente, sizeof(int32_t), MSG_WAITALL);

    if (id_cliente != cliente_esperado)
    {
        perror("Error cliente inválido");
        send(fd_conexion, &resultError, sizeof(int32_t), 0);
        return -1;
    }

    send(fd_conexion, &resultOk, sizeof(int32_t), 0);
    return 0;
}

int32_t crear_conexion(char *ip, char *puerto)
{
    struct addrinfo hints;
    struct addrinfo *server_info;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(ip, puerto, &hints, &server_info) != 0)
    {
        perror("Error al obtener informacion de direcciones");
        return -1;
    }

    int32_t socket_cliente = socket(server_info->ai_family,
                                    server_info->ai_socktype,
                                    server_info->ai_protocol);
    if (socket_cliente == -1)
    {
        perror("Error al crear el socket");
        freeaddrinfo(server_info);
        return -1;
    }

    if (connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1)
    {
        perror("Error al conectar al servidor");
        close(socket_cliente);
        freeaddrinfo(server_info);
        return -1;
    }

    freeaddrinfo(server_info);

    return socket_cliente;
}

int32_t handshake(int32_t fd_conexion, t_cliente id_cliente)
{
    int32_t resultado;

    if (send(fd_conexion, &id_cliente, sizeof(int32_t), 0) == -1)
    {
        perror("Error al enviar el ID del cliente");
        return -1;
    }

    if (recv(fd_conexion, &resultado, sizeof(int32_t), MSG_WAITALL) == -1)
    {
        perror("Error al recibir el resultado del handshake");
        return -1;
    }

    return resultado;
}

void cerrar_conexion(int32_t socket_cliente)
{
    close(socket_cliente);
}