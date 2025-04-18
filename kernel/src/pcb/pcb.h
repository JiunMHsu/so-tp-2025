#ifndef KERNEL_PCB_H
#define KERNEL_PCB_H

#include <stdlib.h>
#include <stdio.h>
#include <commons/collections/dictionary.h>

typedef enum
{
    NEW,
    READY,
    EXEC,
    BLOCKED,
    SUSPENDED_BLOCKED,
    SUSPENDED_READY,
    EXIT
} t_state;

typedef enum
{
    NONE,
    SCHEDULER_INT,
    SYSCALL,
} motivo_desalojo;

// typedef enum
// {
//     SUCCESS,
//     INVALID_RESOURCE,
//     INVALID_INTERFACE,
//     INTERRUPTED_BY_USER,
//     OUT_OF_MEMORY
// } motivo_finalizacion;

typedef struct
{
    u_int32_t pid;
    u_int32_t program_counter;
    u_int32_t tamanio;

    /**
     * @brief Ruta al archivo ejecutable del proceso. (pseudocodigo)
     *
     */
    char *ejecutable;

    t_state estado;

    motivo_desalojo motivo_desalojo;
    char *syscall;

    /**
     * @brief Cantidad de veces que el proceso estuvo en cada estado.
     *
     * key: t_state, value: u_int32_t
     *
     */
    t_dictionary *metricas_estado;

    /**
     * @brief Tiempos que el proceso estuvo en cada estado.
     *
     * key: t_state, value: a determinar (u_int64_t, t_list *)
     *
     * @note No esta claro la unidad de tiempo.
     * @note No esta claro cómo se deben registar.
     *
     * * Lista de tiempos?
     * * Sólo el último?
     * * Suma de todos?
     */
    t_dictionary *metricas_tiempo;
} t_pcb;

t_pcb *crear_pcb(u_int32_t pid, u_int32_t tamanio, char *ejecutable);
void destruir_pcb(t_pcb *pcb);

void actualizar_pcb(t_pcb *pcb, t_pcb *nuevo_pcb);
void set_estado_pcb(t_pcb *pcb, t_state estado);
void set_motivo_desalojo(t_pcb *pcb, motivo_desalojo motivo);

void debug_pcb(t_pcb *pcb);

#endif // KERNEL_PCB_H
