#ifndef CPU_MEM_REQUEST
#define CPU_MEM_REQUEST

#include <stdlib.h>
#include <utils/protocol/protocol.h>

typedef struct 
{
    u_int32_t pid;
    u_int32_t program_counter;
} t_peticion_cpu;

// TODO hacer las funciones para recibir la peticion de memoria
t_peticion_cpu* recibir_peticion_cpu(int32_t fd_conexion);

void destruir_peticion_cpu(t_peticion_cpu *peticion);

#endif // CPU_MEM_REQUEST
