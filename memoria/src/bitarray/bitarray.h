#ifndef MEMORIA_BITARRAY_H
#define MEMORIA_BITARRAY_H

#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <commons/bitarray.h>
#include <pthread.h>
#include <math.h>

#include "config/config.h"
#include "logger/logger.h"

void incializar_bitmap(void);
int32_t asignar_marco(void);
void liberar_marco(u_int32_t marco_a_liberar);
void destruir_bitmap(void);

#endif // MEMORIA_BITARRAY_H
