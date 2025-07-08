#include "cache.h"

static entrada_cache *memoria_cache;
static algoritmo_sustitucion algoritmo;
static u_int32_t cantidad_entradas;
static u_int32_t habilitada;

void inicializar_cache()
{
    cantidad_entradas = get_entradas_cache();

    if (cantidad_entradas != 0)
    {
        habilitada = 0;
    }
    else
    {
        habilitada = 1;

        memoria_cache = malloc(cantidad_entradas * sizeof(entrada_cache));

        algoritmo = get_algoritmo_sustitucion_cache();
    }
}

// TODO: funciones de cache

void agregar_entrada_cache(u_int32_t numero_pagina, int32_t marco)
{
}

void escribir_cache(u_int32_t nro_pagina, u_int32_t offset, void *datos, u_int32_t buffer_size)
{
    entrada_cache *entrada = get_entrada(nro_pagina);
    entrada->bit_modificado = 1;

    memcpy(entrada->contenido + offset, datos, buffer_size);
}

void *leer_cache(u_int32_t nro_pagina, u_int32_t bytes_tamanio)
{
    entrada_cache *entrada = get_entrada(nro_pagina);
    void *datos_leidos = malloc(bytes_tamanio); // TODO LIBERAR

    memcpy(datos_leidos, entrada->contenido, bytes_tamanio);

    return datos_leidos;
}

entrada_cache crear_entrada(u_int32_t pagina, void *contenido, u_int32_t bit_uso, u_int32_t bit_modificado)
{
    entrada_cache nueva_entrada;

    nueva_entrada.pagina = pagina;
    // TODO
    // copia del contenido en nueva_entrada.contenido
    nueva_entrada.bit_uso = 1;
    nueva_entrada.bit_modificado = 0;

    return nueva_entrada;
}

entrada_cache *get_entrada(u_int32_t nro_pagina)
{
    for (u_int32_t i = 0; i < cantidad_entradas; i++)
    {
        if (memoria_cache[i].pagina == nro_pagina)
        {
            memoria_cache[i].bit_uso = 1;
            return &memoria_cache[i];
        }
    }
}

u_int32_t existe_pagina_cache(u_int32_t nro_pagina)
{
    for (u_int32_t i = 0; i < cantidad_entradas; i++)
    {
        entrada_cache entrada = memoria_cache[i];

        if (entrada.pagina == nro_pagina)
            return 1;
    }

    return 0;
}

u_int32_t cache_habilitada()
{
    return habilitada;
}

void destruir_cache() {}
