#include "cache.h"

entrada_cache *memoria_cache;
algoritmo_sustitucion algoritmo;

u_int32_t inicializar_cache()
{
    u_int32_t cantidad_entradas = get_entradas_cache();

    if (cantidad_entradas == 0)
        return 0;

    memoria_cache = malloc(cantidad_entradas * sizeof(entrada_cache));

    algoritmo = get_algoritmo_sustitucion_cache();

    return 1;
}

void agregar_entrada_cache()
{
}

u_int32_t pagina_en_cache(u_int32_t pagina)
{
}

u_int32_t cache_habilitada()
{
}

void escribir_pagina()
{
}

void leer_pagina()
{
}

entrada_cache crear_entrada(u_int32_t pagina, void *contenido, u_int32_t bit_uso, u_int32_t bit_modificado)
{
    entrada_cache nueva_entrada;

    nueva_entrada.pagina = pagina;
    //TODO
    //copia del contenido en nueva_entrada.contenido
    nueva_entrada.bit_uso = 1;
    nueva_entrada.bit_modificado = 0;

    return nueva_entrada;
}