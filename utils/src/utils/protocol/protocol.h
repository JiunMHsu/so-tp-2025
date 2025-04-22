#ifndef UTILS_PROTOCOL_H
#define UTILS_PROTOCOL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <commons/collections/list.h>

typedef enum
{
    PACKET,
    MESSAGE,
    SIGNAL,
} op_code;

typedef struct
{
    int32_t size;
    void *stream;
} t_buffer;

typedef struct
{
    op_code op_code;
    t_buffer *buffer;
} t_packet;

t_packet *crear_paquete(void);
void agregar_a_paquete(t_packet *paquete, void *contenido, int32_t tamanio);
void enviar_paquete(t_packet *paquete, int32_t fd_conexion);
t_list *recibir_paquete(int32_t fd_conexion);
void eliminar_paquete(t_packet *paquete);

void enviar_senial(int32_t signal, int32_t fd_conexion);
int32_t recibir_senial(int32_t fd_conexion);

void enviar_mensaje(char *mensaje, int32_t fd_conexion);

/**
 * @brief Lee un mensaje del socket y lo devuelve como un puntero a char.
 * @attention Aloca memoria para el mensaje recibido.
 *
 * @param fd_conexion
 * @return char*
 */
char *recibir_mensaje(int32_t fd_conexion);

#endif // UTILS_PROTOCOL_H