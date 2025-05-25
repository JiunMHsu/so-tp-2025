#include "instrucciones.h"

t_dictionary *instrucciones;

// TODO: DONDE SE LLAMA A INICIALIZAR INSTRUCCIONES??
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

void noop(char **parametros)
{
    // No hace nada, consume el tiempo que conlleva el fetch y execute
}

void _write(char **parametros)
{
}

void _read(char **parametros)
{
}

void go_to(char **parametros)
{
    // u_int32_t posicion_salto = pasar valor de parametros[0] a int
    // set_program_counter(posicion_salto);
}

void syscall(char **parametros)
{
    set_desalojo();
}

//TODO: implementar logica creacion
instruccion_ejecutable crear_instruccion_ejecutable(char *instruccion)
{
    return (instruccion_ejecutable){NULL, NULL};
}

//TODO: implementar logica destruccion
void destruir_instruccion_ejecutable(instruccion_ejecutable instruccion)
{
    //Podria llegar a tener que liberar mas cosas, por ahora lo implemento para que libere el array de strings de los params
    free(instruccion.parametros);
}