#include "ciclo_instruccion.h"

int8_t hay_syscall;
u_int32_t global_program_counter;

static char *fetch(u_int32_t pid, u_int32_t program_counter);
static instruccion_ejecutable decode(char *instruccion_recibida);
static void execute(instruccion_ejecutable instruccion);

static fin_ejecucion crear_fin_ejecucion(motivo_desalojo motivo, u_int32_t program_counter, char *syscall);

fin_ejecucion ejecutar_ciclo_instruccion(u_int32_t pid, u_int32_t program_counter)
{
    hay_syscall = 0;
    global_program_counter = program_counter;
    fin_ejecucion fin_de_ejecucion;

    while (1)
    {
        char *instruccion_str = fetch(pid, global_program_counter);
        instruccion_ejecutable instruccion = decode(instruccion_str);

        execute(instruccion);
        log_instruccion_ejecutada(pid, instruccion.nombre_instruccion, instruccion.parametros);

        if (hay_syscall)
        {
            fin_de_ejecucion = crear_fin_ejecucion(SYSCALL, global_program_counter, instruccion_str);
            break;
        }

        if (hay_interrupcion())
        {
            fin_de_ejecucion = crear_fin_ejecucion(SCHEDULER_INT, global_program_counter, NULL);
            break;
        }
    }

    destruir_instruccion_ejecutable(instruccion);
    free(instruccion_str);
    return fin_de_ejecucion;
}

static char *fetch(u_int32_t pid, u_int32_t program_counter)
{
    log_fetch_instruccion(pid, program_counter);

    t_peticion_cpu peticion_instruccion = crear_peticion_instruccion(pid, program_counter);
    enviar_peticion_cpu(fd_memoria, peticion_instruccion);
    destruir_peticion_cpu(peticion_instruccion);

    return recibir_mensaje(fd_memoria);
}

static instruccion_ejecutable decode(char *instruccion_recibida)
{
    instruccion_ejecutable instruccion;
    char **vec_instruccion = string_split(instruccion_recibida, " ");

    instruccion.cod_instruccion = strdup(vec_instruccion[0]);
    instruccion.funcion_instruccion = dictionary_get(diccionario_instrucciones, vec_instruccion[0]);
    instruccion.parametros = remove_first_element(vec_instruccion);

    // TODO: traduccion de direcciones de memoria para WRITE y READ -> puede ser que instruccion a ejecutar tenga otro campo "direccion fisica"

    string_array_destroy(vec_instruccion);
    return instruccion;
}

static void execute(instruccion_ejecutable instruccion)
{
    global_program_counter++;
    instruccion.funcion_instruccion(instruccion.parametros);
}

void set_syscall()
{
    hay_syscall = 1;
}

void set_program_counter(u_int32_t valor)
{
    global_program_counter = valor;
}

static fin_ejecucion crear_fin_ejecucion(motivo_desalojo motivo, u_int32_t program_counter, char *syscall)
{
    fin_ejecucion resultado;
    resultado.motivo = motivo;
    resultado.program_counter = program_counter;
    resultado.syscall = syscall ? strdup(syscall) : NULL;

    return resultado;
}
