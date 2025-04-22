#ifndef CPU_CICLO_INSTRUCCION_H
#define CPU_CICLO_INSTRUCCION_H

#include <stdlib.h>

#include "logger/logger.h"
#include "conexion/memoria.h"

void ejecutar_ciclo_instruccion(u_int32_t pid, u_int32_t pc);

#endif // CPU_CICLO_INSTRUCCION_H
