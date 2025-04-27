#include "instrucciones.h"

t_dictionary *instrucciones;

void inicializar_instrucciones()
{
    instrucciones = dictionary_create();

    dictionary_put(instrucciones, "NOOP", &noop);
    dictionary_put(instrucciones, "WRITE", &write);
    dictionary_put(instrucciones, "READ", &read);
    dictionary_put(instrucciones, "GOTO", &go_to);
    dictionary_put(instrucciones, "IO", &io);
    dictionary_put(instrucciones, "INIT_PROC", &init_proc);
    dictionary_put(instrucciones, "DUMP_MEMORY", &dump_memory);
    dictionary_put(instrucciones, "EXIT", &exit_proc);
}

void noop(char **parametros)
{
}

void write(char **parametros)
{
}

void read(char **parametros)
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

void exit_proc(char **parametros)
{
}