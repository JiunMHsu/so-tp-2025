#include "ciclo_instruccion.h"

static char *fetch(u_int32_t pid, u_int32_t program_counter);
static instruccion_ejecutable decode(char *instruccion_recibida);
static void execute(instruccion_ejecutable instruccion, u_int32_t *program_counter);
static int8_t check_desalojo(char *nombre_instruccion);
static int8_t check_interrupt();

fin_ejecucion ejecutar_ciclo_instruccion(u_int32_t pid, u_int32_t program_counter)
{
    while (1)
    {
        // fetch (llamada a memoria)
        // decode (llamada a funciones de instrucciones)
        // execute (incrementar pc y ejecutar funcion instruccion)
        // check desalojo (check syscall, mas prioridad que interrupciones)
        // check interrupt (pregunta a interrupciones)

        // desalojar si hay syscall o si hay interrupcion

        log_fetch_instruccion(pid, program_counter);
        char *instruccion_a_ejecutar = fetch(pid, program_counter);

        instruccion_ejecutable instruccion = decode(instruccion_a_ejecutar); // TODO: tendria que ser un puntero a instruccion_ejecutable?

        // checkeo si lo que estoy por ejecutar es una syscall => si lo es, desalojo
        // aumento program_counter para tenerlo actualizado (porque en realidad eso se hace en execute)
        if (check_desalojo(instruccion) == 1)
        {
            return (fin_ejecucion)
            {
                SYSCALL, program_counter++, instruccion_a_ejecutar
            };
        }

        log_instruccion_ejecutada(pid, instruccion.nombre_instruccion, instruccion.parametros);
        execute(instruccion, &program_counter);

        if (check_interrupt() == 1)
        {
            resetear_interrupcion();
            break;
        }
    }

    return (fin_ejecucion)
    {
        SCHEDULER_INT, program_counter, NULL;
    };
}

// Recibo peticion de ejecucion desde kernell

static char *fetch(u_int32_t pid, u_int32_t program_counter)
{
    t_peticion_cpu peticion_instruccion = crear_peticion_instruccion(pid, program_counter);

    enviar_peticion_cpu(fd_memoria, peticion_instruccion);

    destruir_peticion_cpu(peticion_instruccion);

    char *instruccion_recibida = recibir_mensaje(fd_memoria); // TODO: manejo de errores

    return instruccion_recibida;
}

static instruccion_ejecutable decode(char *instruccion_recibida)
{
    instruccion_ejecutable instruccion;
    char **partes_instruccion = string_split(instruccion_recibida, " ");
    char *clave_instruccion = partes_instruccion[0];
    void *funcion_instruccion = dictionary_get(diccionario_instrucciones, clave_instruccion);
    char **parametros;

    instruccion.funcion_instruccion = funcion_instruccion;

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

static void execute(instruccion_ejecutable instruccion, u_int32_t *program_counter)
{
    if (string_equals_ignore_case(instruccion.nombre_instruccion, "GOTO"))
    {
        instruccion.funcion_instruccion(instruccion.parametros, &program_counter);
    }
    else
    {
        (*program_counter)++;

        instruccion.funcion_instruccion(instruccion.parametros);
    }
}

static int8_t check_desalojo(char *nombre_instruccion)
{
    if (string_equals_ignore_case(nombre_instruccion, "IO") ||
        string_equals_ignore_case(nombre_instruccion, "MEMORY_DUMP") ||
        string_equals_ignore_case(nombre_instruccion, "INIT_PROC") ||
        string_equals_ignore_case(nombre_instruccion, "DUMP_MEMORY") ||
        string_equals_ignore_case(nombre_instruccion, "EXIT"))
    {
        return 1;
    }

    return -1;
}

static int8_t check_interrupt()
{
    return hay_interrupcion();
}
