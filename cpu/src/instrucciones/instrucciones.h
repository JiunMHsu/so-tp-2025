#ifndef CPU_INSTRUCCIONES_H
#define CPU_INSTRUCCIONES_H

#include <stdlib.h>
#include <string.h>
#include <commons/string.h>
#include <commons/collections/dictionary.h>

#include "logger/logger.h"
#include "ciclo_instruccion/ciclo_instruccion.h"
#include "mmu/mmu.h"
#include "cache/cache.h"
#include "conexion/memoria.h"

typedef struct
{
    char *cod_instruccion;
    void (*funcion_instruccion)(char **);
    char **parametros;
} instruccion_ejecutable;

void inicializar_instrucciones(void);

/**
 * @brief Devuelve una función instrucción según el nombre recibido.
 *
 * @param instruccion Ejemplo:
 *        "NOOP", "WRITE", "READ", "GOTO", "IO",
 *        "INIT_PROC", "DUMP_MEMORY", "EXIT".
 *
 * @return `void (*)(char **)` Función instrucción.
 *
 * @note Las funciones instrucción reciben un array de strings de la commons.
 *       Ejemplo: [ "AX", "10", NULL ].
 */
void (*get_instruccion(char *instruccion))(char **);

void destruir_instruccion_ejecutable(instruccion_ejecutable instruccion);

#endif // CPU_INSTRUCCIONES_H
