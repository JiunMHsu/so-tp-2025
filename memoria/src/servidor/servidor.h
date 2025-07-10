#ifndef MEMORIA_SERVIDOR_H
#define MEMORIA_SERVIDOR_H

#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <commons/string.h>
#include <utils/sockets/sockets.h>
#include <utils/protocol/protocol.h>
#include <utils/mem_request/kernel.h>
#include <utils/mem_request/cpu.h>
#include <utils/mem_response/mem_response.h>
#include <utils/mem_datos_paginacion/mem_datos_paginacion.h>

#include "logger/logger.h"
#include "config/config.h"
#include "sistema/sistema.h"
#include "usuario/usuario.h"

void iniciar_servidor(void);
void finalizar_servidor(void);
void finalizar_servidor_por_sigint(int);

#endif // MEMORIA_SERVIDOR_H
