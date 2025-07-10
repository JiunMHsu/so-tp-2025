#include "dump.h"

static char *armar_nombre_archivo(u_int32_t pid);

void generar_dump(u_int32_t pid, t_list *paginas)
{
    log_memory_dump(pid);
    u_int32_t tam_pagina = get_tam_pagina();

    char *nombre_archivo = armar_nombre_archivo(pid);
    FILE *archivo_dump = fopen(nombre_archivo, "w+b");

    if (archivo_dump == NULL)
    {
        log_mensaje_error("No se pudo generar archivo dump.");
        exit(EXIT_FAILURE);
    }

    if (paginas == NULL)
    {
        fclose(archivo_dump);
        free(nombre_archivo);
        return;
    }

    t_list_iterator *iterador_paginas = list_iterator_create(paginas);
    while (list_iterator_has_next(iterador_paginas))
    {
        void *pagina = list_iterator_next(iterador_paginas);
        fwrite(pagina, tam_pagina, 1, archivo_dump);
    }
    list_iterator_destroy(iterador_paginas);

    fclose(archivo_dump);
    free(nombre_archivo);
}

static char *armar_nombre_archivo(u_int32_t pid)
{
    char *full_path = string_new();

    char *path_dumpfile = get_dump_path();
    char *_pid = string_itoa(pid);
    char *timestamp = string_itoa((u_int64_t)time(NULL));

    string_append_with_format(&full_path, "%s%s-%s.dmp", path_dumpfile, _pid, timestamp);

    free(_pid);
    free(timestamp);
    return full_path;
}