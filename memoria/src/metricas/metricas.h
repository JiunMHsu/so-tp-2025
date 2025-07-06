#ifndef METRICAS_H
#define METRICAS_H

#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <commons/collections/dictionary.h>
#include <commons/string.h>
#include <utils/mlist/mlist.h>

typedef struct
{
    u_int32_t accesos_tablas;
    u_int32_t instrucciones_solicitadas;
    u_int32_t swap_outs;
    u_int32_t swap_ins;
    u_int32_t lecturas;
    u_int32_t escrituras;
} t_metricas;

void inicializar_metricas(void);

/**
 * @brief Inicializa `t_metricas` con valores por defecto (0) para un proceso dado.
 *
 * @param pid PID del proceso al que se le crearán las métricas.
 */
void crear_metricas_para(u_int32_t pid);

/**
 * @brief Remueve una métrica del diccionario y la retorna.
 *
 * @param pid PID del proceso al que pertenecen las métricas.
 * @return `t_metricas*`
 */
t_metricas *remover_metricas_para(u_int32_t pid);

/**
 * @brief Destructor de métricas.
 *
 * @param metricas
 * @note Si las métricas son NULL, no hace nada.
 * @note No opera sobre el diccionario global.
 */
void destruir_metricas(t_metricas *metricas);

void incrementar_acceso_tabla(u_int32_t pid);
void incrementar_instruccion_solicitada(u_int32_t pid);
void incrementar_swap_out(u_int32_t pid);
void incrementar_swap_in(u_int32_t pid);
void incrementar_lectura(u_int32_t pid);
void incrementar_escritura(u_int32_t pid);

#endif // METRICAS_H
