#include "sistema.h"

static t_dictionary *procesos_instrucciones;

static u_int8_t hay_espacio_para(u_int32_t tamanio);
static t_list *leer_instrucciones(char *path);

void inicializar_espacio_sistema()
{
    procesos_instrucciones = dictionary_create();

    inicializar_bitmap_estados();
    inicializar_metricas();
    inicializar_tabla_de_paginas();
}

u_int8_t crear_proceso(u_int32_t pid, u_int32_t tamanio, char *path)
{
    if (!hay_espacio_para(tamanio))
        return 0;

    t_list *instrucciones = leer_instrucciones(path);
    if (instrucciones == NULL)
        return 0;

    dictionary_put(procesos_instrucciones, string_itoa(pid), instrucciones);
    crear_tablas_para(pid);
    crear_metricas_para(pid);

    log_creacion_proceso(pid, tamanio);
    return 1;
}

static u_int8_t hay_espacio_para(u_int32_t tamanio)
{
    u_int32_t frames_libres = get_cantidad_frames_disponibles();
    u_int32_t frames_necesarios = (u_int32_t)ceil((double)tamanio / get_tam_pagina());
    if (frames_libres < frames_necesarios)
        return 0;

    return 1;
}

static t_list *leer_instrucciones(char *path)
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

u_int8_t finalizar_proceso(u_int32_t pid)
{
    char *key_pid = string_itoa(pid);

    if (!dictionary_has_key(procesos_instrucciones, key_pid))
    {
        log_mensaje_error("Se inteto finalizar un proceso inexistente.");
        free(key_pid);
        return 0;
    }

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

    return instruccion == NULL ? NULL : strdup(instruccion);
}
