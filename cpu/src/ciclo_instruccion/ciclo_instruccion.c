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
        // fetch (llamada a memoria)
        // decode (llamada a funciones de instrucciones)
        // execute (incrementar pc y ejecutar funcion instruccion)
        // check desalojo (check syscall, mas prioridad que interrupciones)
        // check interrupt (pregunta a interrupciones)

        // desalojar si hay syscall o si hay interrupcion

        log_fetch_instruccion(pid, global_program_counter);
        char *instruccion_a_ejecutar = fetch(pid, global_program_counter);

        instruccion_ejecutable instruccion = decode(instruccion_a_ejecutar);

        log_instruccion_ejecutada(pid, instruccion.nombre_instruccion, instruccion.parametros);
        execute(instruccion);

        if (check_desalojo(instruccion))
        {
            resetear_desalojo();
            destruir_instruccion_ejecutable(instruccion);

            return (fin_ejecucion){SYSCALL, global_program_counter, instruccion_a_ejecutar};
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

// Recibo peticion de ejecucion desde kernell
static char *fetch(u_int32_t pid, u_int32_t program_counter)
{
    t_peticion_cpu peticion_instruccion = crear_peticion_instruccion(pid, program_counter);

    enviar_peticion_cpu(fd_memoria, peticion_instruccion);

    destruir_peticion_cpu(peticion_instruccion);

    char *instruccion_recibida = recibir_mensaje(fd_memoria);

    return instruccion_recibida;
}

static instruccion_ejecutable decode(char *instruccion_recibida)
{
    // TODO: cambiar por una funcion crear_instruccion_ejecutable y otra destruir_instruccion_ejecutable -> alocar memoria manualmente
    instruccion_ejecutable instruccion;
    char **partes_instruccion = string_split(instruccion_recibida, " ");
    char *clave_instruccion = partes_instruccion[0];
    void *funcion_instruccion = dictionary_get(diccionario_instrucciones, clave_instruccion);
    char **parametros;
    instruccion.funcion_instruccion = funcion_instruccion;

    // TODO: implemetar funcion crear_instruccion_ejecutable
    //instruccion_ejecutable instruccion = crear_instruccion_ejecutable(instruccion_recibida); // la instruccion se libera al final del ciclo

    if (string_equals_ignore_case(clave_instruccion, "NOOP"))
    {
        instruccion.parametros = NULL;
        return instruccion;
    }

    // salteo clave de instruccion
    for (int32_t i = 1; i < string_array_size(partes_instruccion); i++)
    {
        string_array_push(parametros, partes_instruccion[i]);
    }

    instruccion.nombre_instruccion = clave_instruccion;
    instruccion.parametros = parametros;

    // TODO: traduccion de direcciones de memoria para WRITE y READ -> puede ser que instruccion a ejecutar tenga otro campo "direccion fisica"
    // podria haber un modulo MMU que tiene las funciones de traduccion -> lo llamo aca adentro de un if que evalua si hay READ o WRITE

    string_array_destroy(partes_instruccion);

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