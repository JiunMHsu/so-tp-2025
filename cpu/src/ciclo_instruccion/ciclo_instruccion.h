#ifndef CPU_CICLO_INSTRUCCION_H
#define CPU_CICLO_INSTRUCCION_H

#include <stdlib.h>
#include <utils/ejecucion/desalojo.h>

#include "logger/logger.h"
#include "conexion/memoria.h"
#include "instrucciones/instrucciones.h"

typedef struct
{
    motivo_desalojo motivo;
    u_int32_t pc;
    char *syscall; // NULL si no hubo syscall
} fin_ejecucion;

fin_ejecucion ejecutar_ciclo_instruccion(u_int32_t pid, u_int32_t pc);

#endif // CPU_CICLO_INSTRUCCION_H
