#include "instrucciones.h"

t_dictionary *instrucciones;

//TODO: DONDE SE LLAMA A INICIALIZAR INSTRUCCIONES??
void inicializar_instrucciones()
{
    instrucciones = dictionary_create();

    dictionary_put(instrucciones, "NOOP", &noop);
    dictionary_put(instrucciones, "WRITE", &_write);
    dictionary_put(instrucciones, "READ", &_read);
    dictionary_put(instrucciones, "GOTO", &go_to);
    dictionary_put(instrucciones, "IO", &io);
    dictionary_put(instrucciones, "INIT_PROC", &init_proc);
    dictionary_put(instrucciones, "DUMP_MEMORY", &dump_memory);
    dictionary_put(instrucciones, "EXIT", &__exit);
}

void noop(char **parametros)
{
}

void _write(char **parametros)
{
}

void _read(char **parametros)
{
}

void go_to(char **parametros)
{
}

void io(char **parametros)
{
}

void init_proc(char **parametros)
{
}

void dump_memory(char **parametros)
{
}

void __exit(char **parametros)
{
}