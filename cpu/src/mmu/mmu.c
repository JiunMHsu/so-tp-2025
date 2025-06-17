#include "mmu.h"

u_int32_t tlb_habilitada;
u_int32_t cache_habilitada;
u_int32_t cantidad_niveles;
u_int32_t cantidad_entradas_tp;
u_int32_t tamanio_pagina;

void inicializar_mmu()
{
    tlb_habilitada = inicializar_tlb();

    cantidad_niveles = get_cantidad_niveles();
    cantidad_entradas_tp = get_cantidad_entradas_tp();
    tamanio_pagina = get_tamanio_pagina();
}

u_int32_t get_direccion_fisica(u_int32_t pid, u_int32_t direccion_logica)
{
    u_int32_t numero_pagina = floor(direccion_logica / tamanio_pagina);
    u_int32_t offset = direccion_logica % tamanio_pagina;
    int32_t marco;

    // revisar cache => si hay cache miss loggear => si hay cache hit devolver direccion fisica (marco * tamaño de pagina + offset)
    // hubo cache miss => revisar tlb => si hay tlb miss loggear => si hay tlb hit devolver direccion fisica (marco * tamaño de pagina + offset)
    // hubo tlb miss => pedir a memoria => devolver direccion fisica (marco * tamaño de pagina + offset)

    marco = cache_habilitada ? get_marco_cache(numero_pagina) : -1;

    if (marco == -1)
    {
        log_cache_miss(pid, numero_pagina);
        marco = tlb_habilitada ? get_marco_tlb(numero_pagina) : -1;

        if (marco == -1)
        {
            log_tlb_miss(pid, numero_pagina);
            marco = obtener_marco_de_memoria(cantidad_niveles, numero_pagina, cantidad_entradas_tp);

            if (tlb_habilitada)
            {
                agregar_entrada_tlb(numero_pagina, marco);
                log_pagina_ingresada_tlb(numero_pagina, marco);
            }
        }
        else
        {
            log_tlb_hit(pid, numero_pagina);

            if (cache_habilitada)
            {
                agregar_entrada_cache(numero_pagina, marco);
                log_pagina_ingresada_cache(pid, numero_pagina);
            }
        }
    }
    else
    {
        log_cache_hit(pid, numero_pagina);
    }

    log_obtener_marco(pid, numero_pagina, marco);

    return marco * tamanio_pagina + offset;
}

u_int32_t obtener_marco_de_memoria(u_int32_t pid, u_int32_t cantidad_niveles, u_int32_t numero_pagina, u_int32_t cantidad_entradas_tp)
{
    u_int32_t entrada_nivel_x;
    u_int32_t *entradas_por_nivel = malloc(cantidad_niveles * sizeof(u_int32_t));

    for (u_int32_t nivel = 1; nivel <= cantidad_niveles; nivel++)
    {
        entrada_nivel_x = floor(numero_pagina / potencia(cantidad_entradas_tp, (cantidad_niveles - nivel))) % cantidad_entradas_tp;

        entradas_por_nivel[nivel - 1] = entrada_nivel_x; // Al nivel 1 le corresponde el indice 0 y así
    }

    // enviar mem request y recibir marco
    // En vez de mandar una pagina se manda un array con las entradas por nivel que debe ir accediendo la memoria para llegar al marco
    // TODO revisar implementacion en mem_request
    enviar_peticion_marco(pid, cantidad_niveles, entradas_por_nivel);

    u_int32_t marco = recibir_marco();

    // TODO no se si va aca, estaria liberando despues de recibir respuesta por ende memoria no deberia resultar perjudicada
    // CUANDO SE DEBERIA LIBERAR ESTE ARRAY??
    free(entradas_por_nivel);

    return marco;
}

// No tengo ganas de romperme la cabeza con problemas de tipo => pow recibe doubles y devuelve doubles, se vuelve medio pesado solucionar
u_int32_t potencia(u_int32_t base, u_int32_t exponente)
{
    u_int32_t resultado = 1;

    for (u_int32_t i = 0; i < exponente; i++)
    {
        resultado *= base;
    }

    return resultado;
}

void destruir_mmu()
{
    destruir_cache();
    destruir_tlb();
}
