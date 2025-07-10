#include "cache.h"

static entrada_cache *memoria_cache;
static algoritmo_sustitucion algoritmo;
static u_int32_t cantidad_entradas;
static u_int32_t habilitada;
static u_int32_t entrada_libre;
static u_int32_t retardo_milisegundos;
static u_int32_t puntero_clock;

void inicializar_cache()
{
    cantidad_entradas = get_entradas_cache();
    retardo_milisegundos = get_retardo();

    if (cantidad_entradas == 0)
    {
        habilitada = 0;
    }
    else
    {
        habilitada = 1;

        memoria_cache = malloc(cantidad_entradas * sizeof(entrada_cache));

        entrada_libre = 0;
        algoritmo = get_algoritmo_sustitucion_cache();
        puntero_clock = 0;
    }
}

// TODO: funciones de cache

void cachear_pagina(u_int32_t nro_pagina, u_int32_t marco)
{
    entrada_cache nueva_entrada = crear_entrada(nro_pagina, marco);

    if (entrada_libre < cantidad_entradas)
    {
        memoria_cache[entrada_libre] = nueva_entrada;
        entrada_libre++;
    }
    else
    {
        u_int32_t indice_victima = obtener_victima();
        entrada_cache victima = get_entrada(indice_victima);

        if (victima.bit_modificado)
        {
            enviar_peticion_escritura_pagina(get_pid(), victima.marco, victima.contenido, get_tamanio_pagina()); // TODO sacar tamanio de pagina
        }

        memoria_cache[indice_victima] = nueva_entrada;
        destruir_entrada_cache(victima);
    }

    log_pagina_ingresada_cache(get_pid(), nro_pagina);
}

u_int32_t obtener_victima()
{
    u_int32_t indice_victima = 0;

    switch (algoritmo)
    {
    case CLOCK:
        while (1)
        {
            if (memoria_cache[puntero_clock].bit_uso == 0)
            {
                indice_victima = puntero_clock;
                puntero_clock = (puntero_clock + 1) % cantidad_entradas; // permite avanzar al siguiente de manera CIRCULAR
                return indice_victima;
            }
            else
            {
                memoria_cache[puntero_clock].bit_uso = 0;
                puntero_clock = (puntero_clock + 1) % cantidad_entradas;
            }
        }
        break;
    case CLOCK_M:
        for (u_int32_t paso = 1; paso <= 4; paso++)
        {
            for (u_int32_t i = 0; i < cantidad_entradas; i++)
            {
                // Para todos los casos el bit de uso tiene que ser 0
                // Si es el 1er o 3er paso del clock => se busca la pareja (uso = 0, modificado = 0) y no se modifica nada de la entrada si no es
                // Si es el 2do o 4to paso del clock => se busca la pareja (uso = 0, modificado = 1) y se modifica uso a 0 si no se encuentra esa pareja

                if (memoria_cache[puntero_clock].bit_uso == 0 &&
                    (((paso == 1 || paso == 3) && memoria_cache[puntero_clock].bit_modificado == 0) ||
                     ((paso == 2 || paso == 4) && memoria_cache[puntero_clock].bit_modificado == 1)))
                {
                    indice_victima = puntero_clock;
                    puntero_clock = (puntero_clock + 1) % cantidad_entradas;
                    return indice_victima;
                }
                else
                {
                    if (paso == 2 || paso == 4)
                        memoria_cache[puntero_clock].bit_uso = 0;

                    puntero_clock = (puntero_clock + 1) % cantidad_entradas;
                }
            }
        }
        break;
    default:
        break;
    }

    return indice_victima;
}

void escribir_cache(u_int32_t nro_pagina, u_int32_t offset, void *datos, u_int32_t buffer_size)
{
    usleep(retardo_milisegundos * 1000);

    entrada_cache *entrada = get_entrada(nro_pagina);
    entrada->bit_modificado = 1;

    memcpy(entrada->contenido + offset, datos, buffer_size);
}

void *leer_cache(u_int32_t nro_pagina, u_int32_t offset, u_int32_t bytes_tamanio)
{
    usleep(retardo_milisegundos * 1000);

    entrada_cache *entrada = get_entrada(nro_pagina);
    void *datos_leidos = malloc(bytes_tamanio);

    memcpy(datos_leidos, entrada->contenido + offset, bytes_tamanio);

    return datos_leidos;
}

entrada_cache crear_entrada(u_int32_t nro_pagina, u_int32_t marco)
{
    entrada_cache nueva_entrada;
    nueva_entrada.contenido = malloc(get_tamanio_pagina());

    nueva_entrada.pagina = nro_pagina;
    nueva_entrada.marco = marco;

    enviar_peticion_lectura_pagina(get_pid(), get_direccion_fisica_por_marco(marco));

    nueva_entrada.contenido = recibir_contenido_pagina();

    nueva_entrada.bit_uso = 1;
    nueva_entrada.bit_modificado = 0;

    return nueva_entrada;
}

entrada_cache *get_entrada(u_int32_t indice)
{
    return &memoria_cache[indice];
}

u_int32_t existe_pagina_cache(u_int32_t nro_pagina)
{
    for (u_int32_t i = 0; i < entrada_libre; i++)
    {
        entrada_cache entrada = memoria_cache[i];

        if (entrada.pagina == nro_pagina)
        {
            log_cache_hit(get_pid(), nro_pagina);
            return 1;
        }
    }

    log_cache_miss(get_pid(), nro_pagina);

    return 0;
}

u_int32_t cache_habilitada()
{
    return habilitada;
}

void limpiar_cache()
{
    for (u_int32_t i = 0; i < entrada_libre; i++)
    {
        entrada_cache entrada = memoria_cache[i];

        if (entrada.bit_modificado == 1)
            enviar_peticion_escritura_pagina(get_pid(), entrada.marco, entrada.contenido);

        destruir_entrada_cache(entrada);
    }

    entrada_libre = 0;
}

void destruir_entrada_cache(entrada_cache entrada)
{
    free(entrada.contenido);
}
