#include "instrucciones.h"

t_dictionary *instrucciones;

// Las instrucciones deben respetar la firma
// void instruccion(char **parametros);

static void noop(char **parametros);
static void _write(char **parametros);
static void _read(char **parametros);
static void go_to(char **parametros);
static void _syscall(char **parametros);

void inicializar_instrucciones()
{
    instrucciones = dictionary_create();

    dictionary_put(instrucciones, "NOOP", &noop);
    dictionary_put(instrucciones, "WRITE", &_write);
    dictionary_put(instrucciones, "READ", &_read);
    dictionary_put(instrucciones, "GOTO", &go_to);
    dictionary_put(instrucciones, "IO", &_syscall);
    dictionary_put(instrucciones, "INIT_PROC", &_syscall);
    dictionary_put(instrucciones, "DUMP_MEMORY", &_syscall);
    dictionary_put(instrucciones, "EXIT", &_syscall);
}

void (*get_instruccion(char *instruccion))(char **)
{
    return dictionary_get(instrucciones, instruccion);
}

static void noop(char **_)
{
    // FUNCIÓN VACÍA
}

static void _write(char **parametros)
{
    u_int32_t direccion_logica = atoi(parametros[0]);
    char *datos = parametros[1];

    if (cache_habilitada())
    {
        u_int32_t nro_pagina = get_nro_pagina(direccion_logica);
        u_int32_t offset = get_offset(direccion_logica);

        if (!existe_pagina_cache(nro_pagina))
        {
            u_int32_t marco_pagina = get_marco(direccion_logica);
            cachear_pagina(nro_pagina, marco_pagina);
        }

        escribir_cache(nro_pagina, offset, (void *)datos, string_length(datos));
    }
    else
    {
        u_int32_t direccion_fisica = get_direccion_fisica(direccion_logica);
        enviar_peticion_escritura(get_pid(), direccion_fisica, (void *)datos, string_length(datos));
        log_operacion_acceso_memoria(get_pid(), ESCRITURA, direccion_fisica, datos);
    }
}

static void _read(char **parametros)
{
    u_int32_t direccion_logica = atoi(parametros[0]);
    u_int32_t tamanio_lectura = atoi(parametros[1]);
    void *datos_leidos = NULL;

    if (cache_habilitada())
    {
        u_int32_t nro_pagina = get_nro_pagina(direccion_logica);

        if (!existe_pagina_cache(nro_pagina))
        {
            u_int32_t marco_pagina = get_marco(direccion_logica);
            cachear_pagina(nro_pagina, marco_pagina);
        }

        datos_leidos = leer_cache(nro_pagina, get_offset(direccion_logica), tamanio_lectura);
    }
    else
    {
        u_int32_t direccion_fisica = get_direccion_fisica(direccion_logica);
        enviar_peticion_lectura(get_pid(), direccion_fisica, tamanio_lectura);
        datos_leidos = recibir_datos_lectura();
        log_operacion_acceso_memoria(get_pid(), LECTURA, direccion_fisica, (char *)datos_leidos);
    }

    printf("Datos leidos: %s", (char *)datos_leidos);
    free(datos_leidos);
}

static void go_to(char **parametros)
{
    set_program_counter(atoi(parametros[0]));
}

static void _syscall(char **parametros)
{
    set_syscall();
}

void destruir_instruccion_ejecutable(instruccion_ejecutable instruccion)
{
    if (instruccion.cod_instruccion != NULL)
        free(instruccion.cod_instruccion);

    if (instruccion.funcion_instruccion != NULL)
        string_array_destroy(instruccion.parametros);
}
