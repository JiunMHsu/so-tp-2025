#ifndef KERNEL_SERVIDOR_H
#define KERNEL_SERVIDOR_H

#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <utils/sockets/sockets.h>
#include <utils/protocol/protocol.h>

#include "config/config.h"
#include "logger/logger.h"
#include "recursos/cpu.h"
#include "recursos/io.h"

void iniciar_servidor(void);
void finalizar_servidor(void);
void finalizar_servidor_por_sigint(int);

#endif // KERNEL_SERVIDOR_H
