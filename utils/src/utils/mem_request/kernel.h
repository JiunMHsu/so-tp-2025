#ifndef KERNEL_MEM_REQUEST_H
#define KERNEL_MEM_REQUEST_H

#include <stdlib.h>
#include <stdint.h>
#include <utils/protocol/protocol.h>

typedef enum
{
    INICIAR_PROCESO,
    FINALIZAR_PROCESO,
    DUMP_PROCESO,
    SWAP_OUT,
    SWAP_IN,
} kernel_req_operation;

typedef struct
{
    kernel_req_operation operacion;
    u_int32_t pid;     // El PID está presente en todos los casos
    u_int32_t tamanio; // Presente solo para INICIAR_PROCESO
    char *path;        // Presente solo para INICIAR_PROCESO
} t_kernel_mem_req;

t_kernel_mem_req *crear_kernel_mem_request(kernel_req_operation operacion,
                                           u_int32_t pid,
                                           u_int32_t tamanio,
                                           char *path);
void enviar_kernel_mem_request(int32_t fd_memoria, t_kernel_mem_req *mem_request);
t_kernel_mem_req *recibir_kernel_mem_request(int32_t fd_kernel);
void destruir_kernel_mem_request(t_kernel_mem_req *mem_request);

#endif // KERNEL_MEM_REQUEST_H
