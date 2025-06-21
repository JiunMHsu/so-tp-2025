#include "mmu.h"

u_int32_t tlb_habilitada;
u_int32_t cache_habilitada;
u_int32_t cantidad_niveles;
u_int32_t cantidad_entradas_tp;
u_int32_t tamanio_pagina;
u_int32_t pagina_en_cache;

void inicializar_mmu()
{
    tlb_habilitada = inicializar_tlb();
    cache_habilitada = inicializar_cache();
    pagina_en_cache = 0;

    cantidad_niveles = get_cantidad_niveles();
    cantidad_entradas_tp = get_cantidad_entradas_tp();
    tamanio_pagina = get_tamanio_pagina();
}

u_int32_t get_direccion_fisica(u_int32_t pid, u_int32_t direccion_logica)
{
    u_int32_t numero_pagina = floor(direccion_logica / tamanio_pagina);
    u_int32_t offset = direccion_logica % tamanio_pagina;
    int32_t marco;

    // revisar cache => si hay cache miss loggear => si hay cache hit escribir/leer ahi mismo (dejar como modificado si esta escrito)
    // hubo cache miss => revisar tlb => si hay tlb miss loggear => si hay tlb hit agregar pagina a cache (si esta habilitada) => pedir pagina a memoria => agregar => si no, pedir marco a memoria calcular direccion => operacion
    // hubo tlb miss => pedir a memoria => agregar pagina a cache (si esta habilitada) => escribir ahi => si no, obtener marco => direccion => operacion

    //TODO esta mal conceptualmente
    marco = cache_habilitada ? get_marco_cache(numero_pagina) : -1;

    //TODO no se si es aca donde se consulta la cache => eso creo que deberia estar pasando por fuera de la mmu
    //la mmu capaz tendria que exponer funciones q obtengan los datos de la dir logica por separado y esta funcion en particular
    //capaz tendria que ser obtener marco nada mas

    if (marco != -1)
    {
        pagina_en_cache = 1;
        log_cache_hit(pid, numero_pagina);
        return marco * tamanio_pagina + offset;
    }

    pagina_en_cache = 0;
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
            pagina_en_cache = 1; 
            log_pagina_ingresada_cache(pid, numero_pagina);
        }
    }

    log_obtener_marco(pid, numero_pagina, marco);

    return marco * tamanio_pagina + offset;
}

u_int32_t obtener_marco_de_memoria(u_int32_t pid, u_int32_t cantidad_niveles, u_int32_t numero_pagina, u_int32_t cantidad_entradas_tp)
{
    u_int32_t entrada_nivel_x;
    char *entradas_por_nivel = string_new();

    for (u_int32_t nivel = 1; nivel <= cantidad_niveles; nivel++)
    {
        entrada_nivel_x = floor(numero_pagina / potencia(cantidad_entradas_tp, (cantidad_niveles - nivel))) % cantidad_entradas_tp;

        string_append_with_format(&entradas_por_nivel, "%d", entrada_nivel_x);

        if (nivel < cantidad_niveles)
            string_append(&entradas_por_nivel, " ");
    }

    enviar_peticion_marco(pid, entradas_por_nivel);

    u_int32_t marco = recibir_marco();

    // TODO revisar si esta bien esta liberacion => entiendo que si
    // porque en el envio se hace una copia del string entonces el original (este) no se libera con la destruccion del pedido
    free(entradas_por_nivel);

    return marco;
}

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
