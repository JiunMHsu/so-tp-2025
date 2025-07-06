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
void crear_metricas_para(u_int32_t pid);
t_metricas *remover_metricas_para(u_int32_t pid);
void destruir_metricas(t_metricas *metricas);

void incrementar_acceso_tabla(u_int32_t pid);
void incrementar_instruccion_solicitada(u_int32_t pid);
void incrementar_swap_out(u_int32_t pid);
void incrementar_swap_in(u_int32_t pid);
void incrementar_lectura(u_int32_t pid);
void incrementar_escritura(u_int32_t pid);

#endif // METRICAS_H
