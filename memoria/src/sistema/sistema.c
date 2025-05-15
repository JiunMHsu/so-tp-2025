#include "sistema.h"

// lista de pids con las instrucciones

t_dictionary *procesos;

// cuando se quiere crear un proceso

// 1. leer el archivo de instrucciones
// 2. parsear las instrucciones a lista ->["NOOP", "WRITE 0 EJEMPLO_DE_ENUNCIADO", ...]
// 3. guardar en diccionario {clave: pid, valor: lista de instrucciones}

char *obtener_instruccion(u_int32_t pid, u_int32_t program_counter)
{
    char pid_str = string_itoa(pid);
    t_list *lista_instrucciones = dictionary_get(procesos, pid_str);

    char *instruccion = (char *)list_get(lista_instrucciones, program_counter);
    free(pid_str);

    return instruccion == NULL ? NULL : strdup(instruccion);
}
