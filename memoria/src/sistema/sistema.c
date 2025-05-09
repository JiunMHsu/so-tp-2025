#include "sistema.h"

// lista de pids con las instrucciones

t_dictionary *procesos;

// cuando se quiere crear un proceso

// 1. leer el archivo de instrucciones
// 2. parsear las instrucciones a lista ->["NOOP", "WRITE 0 EJEMPLO_DE_ENUNCIADO", ...]
// 3. guardar en diccionario {clave: pid, valor: lista de instrucciones}

// TODO hacer la busqueda de instruccion
// si cpu te pide una instruccion segun pid y pc
// - buscar en el diccionario la lista de instrucciones

char *obtener_instruccion(u_int32_t pid, u_int32_t program_counter)
{
    char pid_str[20];
    sprintf(pid_str, "%u", pid);
    t_list* lista_instrucciones = dictionary_get(procesos, pid_str);
    if(lista_instrucciones == NULL)
    {
        return strdup("No se encontraron procesos.");
    }

    if(program_counter >= list_size(lista_instrucciones))
    {
        return strdup("El Program Counter se encuentra fuera de rango.");
    }

    char* instruccion = list_get(lista_instrucciones, program_counter);

    return strdup(instruccion);
}
