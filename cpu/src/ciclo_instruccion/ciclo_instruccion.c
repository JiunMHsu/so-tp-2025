#include "ciclo_instruccion.h"

static char *fetch(u_int32_t pid, u_int32_t program_counter);
static instruccion_ejecutable decode(char *instruccion_recibida);
static void execute(instruccion_ejecutable instruccion, u_int32_t *program_counter);
static int8_t check_desalojo(char *nombre_instruccion);
static int8_t check_interrupt();

int8_t hay_desalojo;
u_int32_t global_program_counter;

fin_ejecucion ejecutar_ciclo_instruccion(u_int32_t pid, u_int32_t program_counter)
{
    hay_desalojo = 0;
    global_program_counter = program_counter;

    while (1)
    {
        char *instruccion_str = fetch(pid, global_program_counter);
        instruccion_ejecutable instruccion = decode(instruccion_str);

        execute(instruccion);
        log_instruccion_ejecutada(pid, instruccion.nombre_instruccion, instruccion.parametros);

        if (check_desalojo(instruccion))
        {
            resetear_desalojo();
            destruir_instruccion_ejecutable(instruccion);

            return (fin_ejecucion){SYSCALL, global_program_counter, instruccion_str};
        }

        if (check_interrupt())
        {
            resetear_interrupcion();
            destruir_instruccion_ejecutable(instruccion);

            break;
        }

        // TODO: implementar funcion destruir_instruccion_ejecutable
        destruir_instruccion_ejecutable(instruccion);
    }

    return (fin_ejecucion){SCHEDULER_INT, global_program_counter, NULL};
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
    // podria haber un modulo MMU que tiene las funciones de traduccion -> lo llamo aca adentro de un if que evalua si hay READ o WRITE

    string_array_destroy(vec_instruccion);
    return instruccion;
}

static void execute(instruccion_ejecutable instruccion)
{
    global_program_counter++;

    instruccion.funcion_instruccion(instruccion.parametros);
}

static int8_t check_desalojo(char *nombre_instruccion)
{
    return hay_desalojo;
}

static int8_t check_interrupt()
{
    return hay_interrupcion();
}

static void set_desalojo()
{
    hay_desalojo = 1;
}

static void resetear_desalojo()
{
    hay_desalojo = 0;
}

static void set_program_counter(u_int32_t valor)
{
    global_program_counter = valor;
}