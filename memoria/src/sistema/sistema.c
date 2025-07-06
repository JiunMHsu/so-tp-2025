#include "sistema.h"

static t_dictionary *procesos_instrucciones;
// static t_dictionary *procesos_tablas;

void inicializar_espacio_sistema()
{
    procesos_instrucciones = dictionary_create();

    inicializar_bitmap_estados();
    inicializar_metricas();
    inicializar_tabla_de_paginas();
}

void crear_proceso(int32_t pid, char *path)
{
    t_list *lista_instrucciones = leer_instrucciones(path);
    dictionary_put(procesos_instrucciones, string_itoa(pid), lista_instrucciones);

    crear_proceso_memoria();
    // t_proceso_memoria *estructura_memoria = 
    // dictionary_put(procesos_tablas, string_itoa(pid), estructura_memoria);

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
        list_add(instrucciones, remove_new_line(linea));

    free(linea);
    fclose(archivo);
    return instrucciones;
}

void finalizar_proceso(int32_t pid)
{
    char *key_pid = string_itoa(pid);

    if (dictionary_has_key(procesos_instrucciones, key_pid) == false)
    {
        log_mensaje_error("Se inteto finalizar un proceso inexistente.");
        free(key_pid);
        return;
    }

    t_list *instrucciones = dictionary_remove(procesos_instrucciones, key_pid);
    list_destroy_and_destroy_elements(instrucciones, free);

    // if (dictionary_has_key(procesos_tablas, key_pid) == false)
    // {

    //     log_mensaje_error("Se inteto finalizar un proceso inexistente.");
    //     free(key_pid);
    //     return;
    // }

    // t_proceso_memoria *tabla_de_proceso = dictionary_remove(procesos_tablas, key_pid);
    // destruir_tabla_de_paginas_para_proceso(tabla_de_proceso->tabla_global);

    // log_destruccion_proceso(pid,
    //                         tabla_de_proceso->accesos_tablas,
    //                         tabla_de_proceso->instrucciones_solicitadas,
    //                         tabla_de_proceso->paginas_en_swap,
    //                         tabla_de_proceso->paginas_en_memoria,
    //                         tabla_de_proceso->lecturas_mem,
    //                         tabla_de_proceso->escrituras_mem);

    // free(tabla_de_proceso);
    free(key_pid);
}

char *obtener_instruccion(u_int32_t pid, u_int32_t program_counter)
{
    char *pid_str = string_itoa(pid);
    t_list *lista_instrucciones = dictionary_get(procesos_instrucciones, pid_str);

    char *instruccion = (char *)list_get(lista_instrucciones, program_counter);
    free(pid_str);

    return instruccion == NULL ? NULL : strdup(instruccion);
}
