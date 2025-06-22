#include "instrucciones.h"

t_dictionary *instrucciones;

// Las instrucciones deben respetar la firma
// void instruccion(char **parametros);

static void noop(char **parametros);
static void _write(char **parametros);
static void _read(char **parametros);
static void go_to(char **parametros);
static void _syscall(char **parametros);

void inicializar_instrucciones()
{
    instrucciones = dictionary_create();

    dictionary_put(instrucciones, "NOOP", &noop);
    dictionary_put(instrucciones, "WRITE", &_write);
    dictionary_put(instrucciones, "READ", &_read);
    dictionary_put(instrucciones, "GOTO", &go_to);
    dictionary_put(instrucciones, "IO", &_syscall);
    dictionary_put(instrucciones, "INIT_PROC", &_syscall);
    dictionary_put(instrucciones, "DUMP_MEMORY", &_syscall);
    dictionary_put(instrucciones, "EXIT", &_syscall);
}

void (*get_instruccion(char *instruccion))(char **)
{
    return dictionary_get(instrucciones, instruccion);
}

static void noop(char **_)
{
    // FUNCIÓN VACÍA
}

static void _write(char **parametros)
{
    // descomponer parametros
    // 1. direccion logica
    // 2. dato a escribir en memoria principal (o memoria cache si es que la pagina se encuentra en ella)

    //fijarse si hay cache habilitada
    //si esta habilitada => operar en cache
    //si NO esta habilitada => operar en memoria => traduccion de direccion logica a fisica

    // operacion en cache_
    // pagina en cache => operar directamente
    // pagina NO esta en cache:
    // CASO 1: tlb habilitada => buscar marco en tlb => pedir a memoria la pagina de ese marco => cargarla en cache => operar en cache
    // CASO 2: tlb NO habilitada => buscar pagina directamente en memoria y cargarla => operar en cache
}

static void _read(char **parametros)
{
    //descomponer parametros
    //1. direccion logica
    //2. tamaño de dato a leer

    // fijarse si hay cache habilitada
    // si esta habilitada => operar en cache
    // si NO esta habilitada => operar en memoria => traduccion de direccion logica a fisica

    //printear el dato leido
    //loggearlo
}

static void go_to(char **parametros)
{
    set_program_counter(atoi(parametros[0]));
}

static void _syscall(char **parametros)
{
    set_syscall();
}

void destruir_instruccion_ejecutable(instruccion_ejecutable instruccion)
{
    if (instruccion.cod_instruccion != NULL)
        free(instruccion.cod_instruccion);

    if (instruccion.funcion_instruccion != NULL)
        string_array_destroy(instruccion.parametros);
}
