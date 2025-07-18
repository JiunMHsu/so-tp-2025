#include "sistema.h"

static t_dictionary *procesos_instrucciones;

static t_list *leer_instrucciones(char *path);

void inicializar_espacio_sistema()
{
    procesos_instrucciones = dictionary_create();

    inicializar_bitmap_estados();
    inicializar_metricas();
    inicializar_tabla_de_paginas();
}

u_int8_t crear_proceso(u_int32_t pid, u_int32_t tamanio, char *ejecutable)
{
    u_int32_t frames_necesarios = (u_int32_t)ceil((double)tamanio / get_tam_pagina());

    t_list *frames_asignados = ocupar_frames(frames_necesarios);
    if (frames_asignados == NULL)
        return 0; // no hay suficientes frames libres

    dictionary_put(procesos_instrucciones, string_itoa(pid), leer_instrucciones(ejecutable));
    crear_metricas_para(pid);
    crear_tablas_para(pid);
    cargar_marcos_asignados(pid, frames_asignados);

    list_destroy_and_destroy_elements(frames_asignados, &free);

    log_creacion_proceso(pid, tamanio);
    return 1;
}

static t_list *leer_instrucciones(char *ejecutable)
{
    char *path_completo = string_new();
    char *path_base = get_path_instrucciones();
    string_append(&path_completo, path_base);
    string_append(&path_completo, ejecutable);

    FILE *archivo = fopen(path_completo, "r");

    if (archivo == NULL)
    {
        log_mensaje_error("No se pudo abrir archivo de instrucciones.");
        exit(EXIT_FAILURE); // no debería ocurrir, paniqueo directo
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

u_int8_t finalizar_proceso(u_int32_t pid)
{
    char *key_pid = string_itoa(pid);

    if (!dictionary_has_key(procesos_instrucciones, key_pid))
    {
        log_mensaje_error("Se inteto finalizar un proceso inexistente.");
        free(key_pid);
        return 0;
    }

    t_list *marcos_asignados = obtener_marcos_asignados(pid);

    if (marcos_asignados == NULL)
        return 1; // no hay marcos asignados, no hay nada que hacer

    liberar_frames(marcos_asignados);

    list_destroy_and_destroy_elements(marcos_asignados, &free);
    t_list *instrucciones = dictionary_remove(procesos_instrucciones, key_pid);
    list_destroy_and_destroy_elements(instrucciones, free);

    t_metricas *metricas = remover_metricas_para(pid);

    log_destruccion_proceso(pid, metricas);

    destruir_tablas_para(pid);
    destruir_metricas(metricas);
    free(key_pid);
    return 1;
}

char *obtener_instruccion(u_int32_t pid, u_int32_t program_counter)
{
    char *pid_str = string_itoa(pid);
    t_list *lista_instrucciones = dictionary_get(procesos_instrucciones, pid_str);

    char *instruccion = (char *)list_get(lista_instrucciones, program_counter);
    free(pid_str);

    if (instruccion != NULL)
    {
        incrementar_instruccion_solicitada(pid);
        return strdup(instruccion);
    }

    return NULL; // no hay instruccion
}

u_int8_t swap_out_proceso(u_int32_t pid)
{
    log_swap_out_solicitado(pid);
    t_list *marcos_asignados = obtener_marcos_asignados(pid);
    if (marcos_asignados == NULL)
    {
        // TODO: revisar que hacer aca (swapeo exitoso?)
        return 1; // no hay marcos asignados, no hay nada que hacer
    }

    t_list *paginas = leer_paginas_por_marcos(marcos_asignados);

    guardar_en_swap(pid, paginas);
    liberar_frames(marcos_asignados);

    incrementar_swap_out(pid);

    list_destroy_and_destroy_elements(marcos_asignados, &free);
    list_destroy_and_destroy_elements(paginas, &free);
    log_swap_out(pid);
    return 1;
}

u_int8_t swap_in_proceso(u_int32_t pid)
{
    log_swap_in_solicitado(pid);
    int32_t cantidad_paginas = get_cantidad_paginas(pid);
    if (cantidad_paginas < 0)
    {
        log_evento("Error al obtener cantidad de paginas para swap in");
        return 0; // error al obtener cantidad de paginas
    }

    if (cantidad_paginas == 0)
        return 1; // no estoy seguro si debería validar este caso

    t_list *marcos_asignados = ocupar_frames(cantidad_paginas);
    if (marcos_asignados == NULL)
    {
        log_evento("No hay suficientes frames libres para swap in");
        return 0; // no hay suficientes frames libres
    }

    t_list *paginas_recuperadas = recuperar_de_swap(pid);
    if (list_size(paginas_recuperadas) != cantidad_paginas)
    {
        log_evento("Error al recuperar paginas de swap");
        list_clean_and_destroy_elements(marcos_asignados, &free);
        list_destroy_and_destroy_elements(paginas_recuperadas, &free);
        return 0; // error al recuperar paginas
    }

    escribir_marcos_enteros(marcos_asignados, paginas_recuperadas);

    // se asume que la cantidad de paginas no cambia, es decir, se reemplazan
    // justo los marcos que se habían asignado antes del swap out
    cargar_marcos_asignados(pid, marcos_asignados);

    incrementar_swap_in(pid);

    list_clean_and_destroy_elements(marcos_asignados, &free);
    list_destroy_and_destroy_elements(paginas_recuperadas, &free);
    log_swap_in(pid);
    return 1;
}

u_int8_t dump_proceso(u_int32_t pid)
{
    t_list *marcos_asignados = obtener_marcos_asignados(pid);
    if (marcos_asignados == NULL)
    {
        generar_dump(pid, NULL);
        return 1;
    }

    t_list *paginas = leer_paginas_por_marcos(marcos_asignados);
    generar_dump(pid, paginas);
    log_evento("Dump del proceso completado");
    return 1;
}
