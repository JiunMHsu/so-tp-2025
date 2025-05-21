#include "sistema.h"

t_dictionary *procesos;

void inicializar_espacio_sistema()
{
    procesos = dictionary_create();
}

void crear_proceso(int32_t pid, char *path)
{
    t_list *lista_instrucciones = leer_instrucciones(path);
    dictionary_put(procesos, string_itoa(pid), lista_instrucciones);
    int tamanio = list_size(lista_instrucciones);
    log_creacion_proceso(pid, tamanio); // TODO: el tamaño es el de espacio de memoria
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

    while (getline(&linea, &len, archivo) != -1)
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

    t_list *instrucciones = dictionary_remove(procesos, key_pid);
    list_destroy_and_destroy_elements(instrucciones, free);
    log_destruccion_proceso(pid);

    free(key_pid);
}

char *obtener_instruccion(u_int32_t pid, u_int32_t program_counter)
{
    char *pid_str = string_itoa(pid);
    t_list *lista_instrucciones = dictionary_get(procesos, pid_str);

    char *instruccion = (char *)list_get(lista_instrucciones, program_counter);
    free(pid_str);

    return instruccion == NULL ? NULL : strdup(instruccion);
}
