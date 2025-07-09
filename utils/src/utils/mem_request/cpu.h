#ifndef CPU_MEM_REQUEST_H
#define CPU_MEM_REQUEST_H

#include <stdlib.h>
#include <commons/string.h>
#include <utils/protocol/protocol.h>

typedef enum
{
    FETCH_INSTRUCCION,
    OBTENER_MARCO, // Acceso a tabla de páginas
    LEER,          // Acceso a espacio de usuario
    ESCRIBIR,      // Acceso a espacio de usuario
    LEER_PAG,      // Acceso a espacio de usuario
    ESCRIBIR_PAG   // Acceso a espacio de usuario
} operacion_cpu_memoria;

typedef struct
{
    operacion_cpu_memoria operacion;
    u_int32_t pid;
    u_int32_t program_counter; // Presente solo para FETCH_INSTRUCCION

    /**
     * @brief "1 2 3 4" -> {1, 2, 3, 4}
     *
     */
    char *entradas_por_nivel;    // Presente solo para OBTENER_MARCO
    u_int32_t direccion_fisica; // Presente para LEER, ESCRIBIR, LEER_PAG y ESCRIBIR_PAG
    u_int32_t tamanio_buffer;    // Presente para LEER, ESCRIBIR, LEER_PAG y ESCRIBIR_PAG
    void *buffer;                // Presente solo para ESCRIBIR y ESCRIBIR_PAG
} t_peticion_cpu;

t_peticion_cpu *crear_peticion_instruccion(u_int32_t pid, u_int32_t program_counter);
t_peticion_cpu *crear_peticion_nro_marco(u_int32_t pid, char *entradas_por_nivel); // TODO: Revisar
t_peticion_cpu *crear_peticion_lectura(u_int32_t pid, u_int32_t direccion_fisica, u_int32_t tamanio_buffer);
t_peticion_cpu *crear_peticion_escritura(u_int32_t pid, u_int32_t direccion_fisica, u_int32_t tamanio_buffer, void *buffer);

void enviar_peticion_cpu(int32_t fd_memoria, t_peticion_cpu *peticion);
t_peticion_cpu *recibir_peticion_cpu(int32_t fd_conexion);
void destruir_peticion_cpu(t_peticion_cpu *peticion);

t_list *convertir_a_lista_entradas_por_nivel(char *entradas_por_nivel);

#endif // CPU_MEM_REQUEST_H
