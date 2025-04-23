#ifndef CPU_INTERRUPCION_H
#define CPU_INTERRUPCION_H

#include <stdlib.h>
#include <pthread.h>
#include <utils/protocol/protocol.h>

void inicializar_interrupcion(int32_t fd_interrupt);
int8_t hay_interrupcion(void);
void resetear_interrupcion(void);

#endif // CPU_INTERRUPCION_H
