#include "tlb.h"

entrada_tlb **tlb;
algoritmo_sustitucion algoritmo;

u_int32_t inicializar_tlb()
{
    u_int32_t cantidad_entradas = get_entradas_tlb();

    if (cantidad_entradas == 0) return 0;

    algoritmo = get_algoritmo_sustitucion();

    tlb = malloc(cantidad_entradas * sizeof(entrada_tlb));

    return 1;
}

void agregar_entrada(u_int32_t pagina, u_int32_t marco)
{
}

int32_t get_marco_tlb(u_int32_t pagina)
{
    int32_t indice = obtener_indice(pagina);

    if(indice == -1) return -1; //tlb miss

    return marco; //tlb hit
}

int32_t obtener_indice(u_int32_t pagina)
{
    entrada_tlb entrada;

    for(u_int32_t i = 0; i <= cantidad_entradas; i++)
    {
        entrada = tlb[i];

        if(entrada.pagina == pagina) return i;
    }

    return -1;
}

void tlb_destroy()
{
    free(tlb); // por ahora no tiene info a liberar dentro
}
