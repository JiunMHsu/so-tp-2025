
#ifndef KERNEL_SERVIDOR_H
#define KERNEL_SERVIDOR_H

#include <stdlib.h>
#include <pthread.h>
#include <commons/string.h>
#include <utils/sockets/sockets.h>
#include <utils/protocol/protocol.h>

// #include "logger/logger.h"
// #include "config/config.h"
// #include "planificador/planificador.h"

void iniciar_servidor(void);
void finalizar_servidor(void);

#endif // KERNEL_SERVIDOR_H
