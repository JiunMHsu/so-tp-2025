#ifndef MEMORIA_USUARIO_H
#define MEMORIA_USUARIO_H

#include <stdio.h>
#include <stdlib.h>
#include <commons/collections/dictionary.h>
#include <commons/collections/list.h>
#include <commons/bitarray.h>

#include "config/config.h"

typedef struct 
{
    void *memoria_principal;
    t_bitarray *asignacion_marcos;
    int32_t cantidad_marcos;
    int32_t tam_pagina;
    FILE *archivo_swap;
}t_memoria;

void inicializar_memoria_principal(t_memoria *memoria);
void asignar_marco();
void liberar_marco();

#endif // MEMORIA_USUARIO_H
