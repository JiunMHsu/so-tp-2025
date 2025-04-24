#ifndef MEMORIA_SERVIDOR_H
#define MEMORIA_SERVIDOR_H

#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <commons/string.h>
#include <utils/sockets/sockets.h>
#include <utils/protocol/protocol.h>

#include "logger/logger.h"
#include "config/config.h"
#include "cliente/cpu.h"
#include "cliente/kernel.h"

void iniciar_servidor(void);
void finalizar_servidor(void);
void finalizar_servidor_por_sigint(int);

#endif // MEMORIA_SERVIDOR_H
