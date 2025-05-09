#ifndef CPU_INSTRUCCIONES_H
#define CPU_INSTRUCCIONES_H

#include <stdlib.h>
#include <string.h>
#include <commons/string.h>
#include <commons/collections/dictionary.h>

#include "logger/logger.h"

typedef struct
{
    void *funcion_instruccion;
    char **parametros;
} instruccion_ejecutable;

void inicializar_instrucciones(void);
void noop(char **parametros);
void _write(char **parametros);
void _read(char **parametros);
void go_to(char **parametros);
void io(char **parametros);
void init_proc(char **parametros);
void dump_memory(char **parametros);
void _exit(char **parametros);

// Las instrucciones deben respetar la firma
// void instruccion(char **parametros);

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

#endif // CPU_INSTRUCCIONES_H
