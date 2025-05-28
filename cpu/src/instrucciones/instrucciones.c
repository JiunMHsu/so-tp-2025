#include "instrucciones.h"

t_dictionary *instrucciones;

static void noop(char **parametros);
static void _write(char **parametros);
static void _read(char **parametros);
static void go_to(char **parametros);
static void syscall(char **parametros);

void inicializar_instrucciones()
{
    instrucciones = dictionary_create();

    dictionary_put(instrucciones, "NOOP", &noop);
    dictionary_put(instrucciones, "WRITE", &_write);
    dictionary_put(instrucciones, "READ", &_read);
    dictionary_put(instrucciones, "GOTO", &go_to);
    dictionary_put(instrucciones, "IO", &syscall);
    dictionary_put(instrucciones, "INIT_PROC", &syscall);
    dictionary_put(instrucciones, "DUMP_MEMORY", &syscall);
    dictionary_put(instrucciones, "EXIT", &syscall);
}

static void noop(char **_)
{
    // FUNCIÓN VACÍA
}

static void _write(char **parametros)
{
}

static void _read(char **parametros)
{
}

static void go_to(char **parametros)
{
    set_program_counter(atoi(parametros[0]));
}

static void syscall(char **parametros)
{
    set_syscall();
}

void destruir_instruccion_ejecutable(instruccion_ejecutable instruccion)
{
    free(instruccion.cod_instruccion);
    string_array_destroy(instruccion.parametros);
}
