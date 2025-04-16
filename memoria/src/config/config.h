#ifndef MEMORIA_CONFIG_H
#define MEMORIA_CONFIG_H

#include <errno.h>
#include <commons/config.h>
#include <commons/log.h>

#define CONFIG_FILE "memoria.config"

void iniciar_config(void);
void destruir_config(void);

char *get_puerto_escucha(void);
t_log_level get_log_level(void);

#endif // MEMORIA_CONFIG_H