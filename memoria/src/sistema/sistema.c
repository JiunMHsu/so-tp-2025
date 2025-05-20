#include "sistema.h"
// TODO
//  lista de pids con las instrucciones

t_dictionary *procesos;

// cuando se quiere crear un proceso

// Nico
//  1. leer el archivo de instrucciones
//  2. parsear las instrucciones a lista ->["NOOP", "WRITE 0 EJEMPLO_DE_ENUNCIADO", ...]
//  3. guardar en diccionario {clave: pid, valor: lista de instrucciones}
void iniciar_proceso(int32_t pid, char *path)
{
    t_list *lista_instrucciones = leer_instrucciones(path);
    dictionary_put(procesos, string_itoa(pid), lista_instrucciones);
    int tamanio = list_size(lista_instrucciones);
    log_creacion_proceso(pid, tamanio);
}

t_list *leer_instrucciones(char *path)
{
    FILE *archivo = fopen(path, "r");

    if (archivo == NULL)
    {
        log_mensaje_error("No se pudo abrir archivo de instrucciones.");
        return NULL;
    }

    char *linea = NULL;
    size_t len = 0;
    t_list *instrucciones = list_create();
    getline while (getline(&linea, &len, archivo) != -1)
    {
        list_add(instrucciones, remove_new_line(linea));
    }

    free(linea);
    fclose(archivo);

    return instrucciones;
}

void finalizar_proceso(int32_t pid)
{
    char *key_pid = string_itoa(pid);

    if (dictionary_has_key(procesos, key_pid) == false)
    {
        log_mensaje_error("Se inteto finalizar un proceso inexistente.");
        free(key_pid);
        return;
    }

    t_list *instrucciones = dictionary_get(procesos, key_pid);

    list_destroy_and_destroy_elements(instrucciones, free);

    log_destruccion_proceso(pid);

    free(key_pid);
}

// Ale
//  si cpu te pide una instruccion segun pid y pc
//  - buscar en el diccionario la lista de instrucciones
