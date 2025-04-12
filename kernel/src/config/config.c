#include "config.h"

t_config *kernel_config;

void iniciar_config(void)
{
    kernel_config = config_create(CONFIG_FILE);
}

void destruir_config(void)
{
    config_destroy(kernel_config);
}

char *get_puerto_escucha(puerto_escucha puerto)
{
    char *key = NULL;
    switch (puerto)
    {
    case CPU_DISPATCH:
        key = "PUERTO_ESCUCHA_DISPATCH";
        break;
    case CPU_INTERRUPT:
        key = "PUERTO_ESCUCHA_INTERRUPT";
        break;
    case IO:
        key = "PUERTO_ESCUCHA_IO";
        break;
    }

    return config_get_string_value(kernel_config, key);
}

mem_address get_memoria_config(void) {}

algoritmo_planificacion get_alg_plani_corto_plazo(void) {}

algoritmo_planificacion get_alg_plani_largo_plazo(void) {}

u_int32_t get_tiempo_suspension(void) {}

t_log_level get_log_level(void) {}
