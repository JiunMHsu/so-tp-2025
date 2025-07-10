#include "tlb.h"

static entrada_tlb *tlb;
static algoritmo_sustitucion algoritmo;
static u_int32_t entrada_libre;
static u_int32_t cantidad_entradas;

static int32_t obtener_indice(u_int32_t pagina);
static entrada_tlb get_entrada(u_int32_t indice_entrada);
static entrada_tlb crear_entrada(u_int32_t pagina, u_int32_t marco);
static void insertar_entrada(entrada_tlb entrada);
static entrada_tlb remover_entrada(u_int32_t indice_pagina);

u_int32_t inicializar_tlb()
{
    cantidad_entradas = get_entradas_tlb();

    if (cantidad_entradas == 0)
        return 0;

    algoritmo = get_algoritmo_sustitucion_tlb();

    tlb = malloc(cantidad_entradas * sizeof(entrada_tlb));
    entrada_libre = 0;

    return 1;
}

u_int32_t get_marco_tlb(u_int32_t pagina)
{
    int32_t indice = obtener_indice(pagina);
    entrada_tlb entrada_obtenida;

    if (indice == -1)
        return -1; // tlb miss

    switch (algoritmo)
    {
    case FIFO:
        entrada_obtenida = get_entrada(indice);
        break;
    case LRU:
        // da la prioridad por acceso => si se accede se pone ultimo => no se sacara al principio
        entrada_obtenida = remover_entrada(indice);
        insertar_entrada(entrada_obtenida);
        break;
    default:
        break;
    }

    return entrada_obtenida.marco; // tlb hit
}

static entrada_tlb get_entrada(u_int32_t indice_entrada)
{
    return tlb[indice_entrada];
}

static void insertar_entrada(entrada_tlb entrada)
{
    tlb[entrada_libre] = entrada;
    entrada_libre++;
}

void agregar_entrada_tlb(u_int32_t pagina, u_int32_t marco)
{
    entrada_tlb entrada = crear_entrada(pagina, marco);

    if (entrada_libre == cantidad_entradas)
        remover_entrada(0);

    tlb[entrada_libre] = entrada;

    entrada_libre++;
}

static entrada_tlb remover_entrada(u_int32_t indice_pagina)
{
    entrada_tlb entrada_removida = tlb[indice_pagina];

    entrada_libre--;

    for (u_int32_t i = indice_pagina; i < entrada_libre; i++)
    {
        entrada_tlb entrada_realocada = tlb[i + 1];
        tlb[i] = entrada_realocada;
    }

    return entrada_removida;
}

static int32_t obtener_indice(u_int32_t pagina)
{
    entrada_tlb entrada;

    for (u_int32_t i = 0; i <= cantidad_entradas; i++)
    {
        entrada = tlb[i];

        if (entrada.pagina == pagina)
            return i;
    }

    return -1;
}

static entrada_tlb crear_entrada(u_int32_t pagina, u_int32_t marco)
{
    entrada_tlb nueva_entrada;

    nueva_entrada.pagina = pagina;
    nueva_entrada.marco = marco;

    return nueva_entrada;
}

void limpiar_tlb()
{
    entrada_libre = 0;
}
