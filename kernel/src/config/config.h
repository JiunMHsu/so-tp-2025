#ifndef KERNEL_CONFIG_H
#define KERNEL_CONFIG_H

#include <stdlib.h>
#include <string.h>
#include <commons/config.h>
#include <commons/log.h>

#define CONFIG_FILE "kernel.config"

typedef enum
{
    ESCUCHA_CPU_DISPATCH,
    ESCUCHA_CPU_INTERRUPT,
    ESCUCHA_IO
} puerto_escucha;

typedef struct
{
    char *ip;
    char *puerto;
} mem_address;

typedef enum
{
    /**
     * @brief (FIFO) First In First Out.
     * No hay nada que explicar aca.
     *
     */
    FIFO,

    /**
     * @brief (SJF) Shortest Job First.
     * Evalúa (estima) el tiempo de ejecución de los procesos en READY
     * y elige el de menor ráfaga.
     *
     */
    SJF,

    /**
     * @brief (SRT) Shortest Remaining Time.
     * Variante de SJF que agrega el desalojo de procesos.
     *
     * Replanifica cada vez que llega un proceso nuevo a READY,
     * o cuando un proceso deja la CPU.
     *
     */
    SRT,

    /**
     * @brief (SPF) Smallest Process First.
     * Nombre inventado, en la consigna se enuncia como
     * "Proceso más chico primero".
     *
     */
    SPF
} algoritmo_planificacion;

void iniciar_config(void);
void destruir_config(void);

char *get_puerto_escucha(puerto_escucha puerto);
mem_address get_memoria_config(void);
algoritmo_planificacion get_alg_plani_corto_plazo(void);
algoritmo_planificacion get_alg_plani_largo_plazo(void);

/**
 * @brief Tiempo en milisegundos que se debe esperar
 * antes de mover un proceso al estado SUSP. BLOCKED.
 *
 * @return u_int32_t (milisegundos)
 */
u_int32_t get_tiempo_suspension(void);
t_log_level get_log_level(void);

#endif // KERNEL_CONFIG_H