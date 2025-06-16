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

    marco = cache_habilitada ? get_marco_cache() : -1;

    if (marco == -1)
    {
        log_cache_miss(pid, numero_pagina);
        marco = tlb_habilitada ? get_marco_tlb() : -1;

        if (marco == -1)
        {
            log_tlb_miss(pid, numero_pagina);
            marco = obtener_marco_memoria(cantidad_niveles, numero_pagina, cantidad_entradas_tp);
        }
        else
        {
            log_tlb_hit(pid, numero_pagina);
        }
    }
    else
    {
        log_cache_miss(pid, numero_pagina);
    }

    log_obtener_marco(pid, numero_pagina, marco);

    return marco * tamanio_pagina + offset;
}

u_int32_t obtener_marco_memoria(u_int32_t pid, u_int32_t cantidad_niveles, u_int32_t numero_pagina, u_int32_t cantidad_entradas_tp)
{
    u_int32_t entrada_nivel_x;
    u_int32_t **entradas_por_nivel = malloc(cantidad_niveles * sizeof(u_int32_t));

    // TODO revisar si empieza en 1 o en 0, los niveles van de 1 en adelante
    for (u_int32_t nivel = 0; nivel <= cantidad_niveles; nivel++)
    {
        entrada_nivel_x = floor(numero_pagina / cantidad_entradas_tp ^ (cantidad_niveles - nivel)) % cantidad_entradas_tp;
        entradas_por_nivel[nivel] = entrada_nivel_x;
    }

    // enviar mem request y recibir marco
    t_peticion_cpu *peticion_marco = crear_peticion_nro_marco(pid, numero_pagina);

    u_int32_t marco = recibir_marco();

    // TODO no se si va aca, estaria liberando despues de recibir respuesta por ende memoria no deberia resultar perjudicada
    free(entradas_por_nivel);

    return marco;
}

void destruir_mmu()
{
    destruir_cache();
    destruir_tlb();
}
