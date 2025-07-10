#include "mmu.h"

static u_int32_t tlb_esta_habilitada;
static u_int32_t cantidad_niveles;
static u_int32_t cantidad_entradas_tp;
static u_int32_t tamanio_pagina;

static u_int32_t obtener_marco_de_memoria(u_int32_t pid,
                                          u_int32_t cantidad_niveles,
                                          u_int32_t numero_pagina,
                                          u_int32_t cantidad_entradas_tp);

static u_int32_t potencia(u_int32_t base, u_int32_t exponente);

void inicializar_mmu()
{
    tlb_esta_habilitada = inicializar_tlb();

    cantidad_niveles = get_cantidad_niveles();
    cantidad_entradas_tp = get_cantidad_entradas_tp();
    tamanio_pagina = get_tamanio_pagina();
}

// TODO cambiar todas las firmas con pid => agregar get_pid de ciclo_instruccion
u_int32_t get_marco(u_int32_t direccion_logica)
{
    u_int32_t numero_pagina = get_nro_pagina(direccion_logica);
    u_int32_t offset = get_offset(direccion_logica);
    u_int32_t pid = get_pid();
    int32_t marco;

    marco = tlb_habilitada ? get_marco_tlb(numero_pagina) : -1;

    if (marco == -1)
    {
        marco = obtener_marco_de_memoria(pid, cantidad_niveles, numero_pagina, cantidad_entradas_tp);

        if (tlb_habilitada)
        {
            log_tlb_miss(pid, numero_pagina);
            agregar_entrada_tlb(numero_pagina, marco);
            log_pagina_ingresada_tlb(numero_pagina, marco);
        }
    }
    else
    {
        log_tlb_hit(pid, numero_pagina);
    }

    log_obtener_marco(pid, numero_pagina, marco);
    return marco;
}

u_int32_t get_direccion_fisica(u_int32_t direccion_logica)
{
    return get_marco(direccion_logica) * tamanio_pagina + get_offset(direccion_logica);
}

u_int32_t get_direccion_fisica_por_marco(u_int32_t marco)
{
    return marco * tamanio_pagina;
}

static u_int32_t obtener_marco_de_memoria(u_int32_t pid, u_int32_t cantidad_niveles, u_int32_t numero_pagina, u_int32_t cantidad_entradas_tp)
{
    u_int32_t entrada_nivel_x;
    char *entradas_por_nivel = string_new();

    for (u_int32_t nivel = 1; nivel <= cantidad_niveles; nivel++)
    {
        entrada_nivel_x = (int)floor(numero_pagina / potencia(cantidad_entradas_tp, (cantidad_niveles - nivel))) % cantidad_entradas_tp;

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

static u_int32_t potencia(u_int32_t base, u_int32_t exponente)
{
    u_int32_t resultado = 1;

    for (u_int32_t i = 0; i < exponente; i++)
        resultado *= base;

    return resultado;
}

u_int32_t get_nro_pagina(u_int32_t direccion_logica)
{
    return (u_int32_t)floor(direccion_logica / tamanio_pagina);
}

u_int32_t get_offset(u_int32_t direccion_logica)
{
    return direccion_logica % tamanio_pagina;
}

u_int32_t tlb_habilitada()
{
    return tlb_esta_habilitada;
}
