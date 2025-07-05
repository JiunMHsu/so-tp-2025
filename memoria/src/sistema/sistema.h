#ifndef MEMORIA_SISTEMA_H
#define MEMORIA_SISTEMA_H

#include <stdio.h>
#include <stdlib.h>
#include <commons/string.h>
#include <commons/collections/dictionary.h>
#include <commons/collections/list.h>
#include <utils/mem_request/kernel.h>
#include <utils/string/string.h>

#include "logger/logger.h"
#include "tabla_paginas/tabla_paginas.h"

void inicializar_espacio_sistema(void);

void crear_proceso(int32_t pid, char *path);
t_list *leer_instrucciones(char *path);
void finalizar_proceso(int32_t pid);

char *obtener_instruccion(u_int32_t pid, u_int32_t program_counter);

#endif // MEMORIA_SISTEMA_H
