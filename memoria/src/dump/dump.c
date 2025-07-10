#include "dump.h"

// TODO: Implementar la función generar_dump
void generar_dump(u_int32_t pid, t_list *paginas)
{
    log_memory_dump(pid);

    u_int32_t tam_pagina = get_tam_pagina();

    // generar el nombre del archivo de dump
    // crear el archivo de dump
    char *nombre_archivo = armar_nombre_archivo(pid);
    FILE *archivo_dump = fopen(nombre_archivo, "w+b");

    if (archivo_dump == NULL)
    {
        log_mensaje_error("No se pudo generar archivo dump.");
        exit(EXIT_FAILURE);
    }

    // retornar si la lista de páginas es NULL (generar archivo vacío)
    if (list_is_empty(paginas))
    {
        fclose(archivo_dump);
        return NULL;
    }

    // iterar sobre la lista de páginas
    // ir escribiendo las páginas en el archivo de dump
    t_list_iterator *iterador_paginas = list_iterator_create(paginas);

    while (list_iterator_has_next(iterador_paginas))
    {
        void *pagina = list_iterator_next(iterador_paginas);
        fwrite(pagina, tam_pagina, 1, archivo_dump);
    }

    list_iterator_destroy(iterador_paginas);

    // cerrar el archivo de dump
    fclose(archivo_dump);
}

chat *armar_nombre_archivo(u_int32_t pid)
{
    char *path_dumpfile = get_dump_path();
    char *_pid = string_itoa(pid);

    // TODO Revisar implementacion
    time_t now = time(NULL);
    struct tm tm_info;
    localtime_r(&now, &tm_info);
    char _time_stamp[32];
    strftime(_time_stamp, sizeof(_time_stamp), "%Y%m%d_%H%M%S", &tm_info);

    size_t len_nombre = strlen(path_dumpfile) + strlen(_pid) + 1 + strlen(_time_stamp) + strlen(".dmp") + 1;

    char *nombre_archivo = malloc(len_nombre);

    snprintf(nombre_archivo, len_nombre, "%s%s-%s.dmp", path_dumpfile, _pid, _time_stamp);

    free(_pid);

    return nombre_archivo;
}