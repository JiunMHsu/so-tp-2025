#include "usuario.h"

void inicializar_memoria_principal(t_memoria* memoria)
{
    int32_t tam_memoria = get_tam_memoria();
    if(tam_memoria <= 0)
    {

    }

    memoria->tam_pagina = get_tam_pagina();
    if(memoria->tam_pagina <= 0)
    {

    }

    memoria->cantidad_marcos = tam_memoria / memoria->tam_pagina;
    memoria->memoria_principal = malloc(tam_memoria);
    if(memoria->memoria_principal == NULL)
    {

    }

    int32_t bytes_asignacion_marcos = ceil(memoria->cantidad_marcos / 8);
    char * buffer = malloc (bytes_asignacion_marcos);
    if(buffer == NULL)
    {

    }

    memset(buffer, 0, bytes_asignacion_marcos);
    memoria->asignacion_marcos = bitarray_create_with_mode(buffer, bytes_asignacion_marcos, LSB_FIRST);
}

int32_t asignar_marco(t_memoria *memoria)
{
    for (int32_t i = 0; i < memoria->cantidad_marcos; i++)
    {
        int32_t bit_asignacion = bitarray_test_bit(memoria->asignacion_marcos, i);

        if (bit_asignacion == 0)
        {
            bitarray_set_bit(memoria->asignacion_marcos, i);

            return i;
        }
    }

    // Caso en el que todos los marcos esten ocupados

    return -1;
}

void liberar_marco(t_memoria *memoria, int32_t marco_a_liberar)
{
    int32_t bit_asignacion = bitarray_test_bit(memoria->asignacion_marcos, marco_a_liberar);
    if (marco_a_liberar < 0 || marco_a_liberar >= memoria->cantidad_marcos)
    {
    }

    if (bit_asignacion == 0)
    {
    }

    bitarray_clean_bit(memoria->asignacion_marcos, marco_a_liberar);
}