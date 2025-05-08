#ifndef MEMORIA_SISTEMA_H
#define MEMORIA_SISTEMA_H

#include <stdio.h>
#include <stdlib.h>
#include <commons/collections/dictionary.h>
#include <commons/collections/list.h>

char* obtener_instruccion(u_int32_t pid, u_int32_t program_counter);

#endif // MEMORIA_SISTEMA_H
