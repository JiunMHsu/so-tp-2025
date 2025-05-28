#ifndef CPU_CICLO_INSTRUCCION_H
#define CPU_CICLO_INSTRUCCION_H

#include <stdlib.h>
#include <utils/ejecucion/desalojo.h>
#include <utils/string/string.h>

#include "logger/logger.h"
#include "conexion/kernel.h"
#include "conexion/memoria.h"
#include "instrucciones/instrucciones.h"
#include "interrupcion/interrupcion.h"

typedef struct
{
    motivo_desalojo motivo;
    u_int32_t program_counter;
    char *syscall; // NULL si no hubo syscall
} fin_ejecucion;

fin_ejecucion ejecutar_ciclo_instruccion(u_int32_t pid, u_int32_t pc);

void set_syscall(void);
void set_program_counter(u_int32_t valor);

#endif // CPU_CICLO_INSTRUCCION_H
