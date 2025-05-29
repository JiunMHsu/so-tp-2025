#ifndef KERNEL_PCB_H
#define KERNEL_PCB_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <commons/temporal.h>
#include <commons/collections/dictionary.h>
#include <commons/collections/list.h>

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

    /**
     * @brief Cantidad de veces que el proceso estuvo en cada estado.
     *
     * @note key: `t_state`, value: `u_int32_t`
     *
     */
    t_dictionary *metricas_estado;

    /**
     * @brief Tiempos que el proceso estuvo en cada estado.
     *
     * @note key: `t_state`, value: `t_list *` (de `u_int64_t`)
     */
    t_dictionary *metricas_tiempo;

    t_temporal *temporal;

    u_int64_t ultima_estimacion_rafaga; // en milisegundos
    u_int64_t estimacion_rafaga;        // en milisegundos

    pthread_mutex_t mutex;
} t_pcb;

char *get_estado_string(t_state estado);

t_pcb *crear_pcb(u_int32_t pid, u_int32_t tamanio, char *ejecutable, u_int64_t est_rafaga_inicial);
void destruir_pcb(t_pcb *pcb);

t_state get_estado_pcb(t_pcb *pcb);
u_int64_t get_ultima_estimacion_rafaga_pcb(t_pcb *pcb);
u_int64_t get_estimacion_rafaga_pcb(t_pcb *pcb);

// TODO: get de las métricas de estado y tiempo (ojo el caso EXIT que no transita a otro)

void set_program_counter_pcb(t_pcb *pcb, u_int32_t program_counter);
void set_estado_pcb(t_pcb *pcb, t_state estado);
void set_ultima_estimacion_rafaga_pcb(t_pcb *pcb, u_int64_t ultima_estimacion);
void set_estimacion_rafaga_pcb(t_pcb *pcb, u_int64_t estimacion);

void debug_pcb(t_pcb *pcb);

#endif // KERNEL_PCB_H
