#include "config.h"

t_config *kernel_config;

void iniciar_config()
{
    kernel_config = config_create(CONFIG_FILE);
}

void destruir_config()
{
    config_destroy(kernel_config);
}

char *get_puerto_escucha(puerto_escucha puerto)
{
    char *key = NULL;
    switch (puerto)
    {
    case ESCUCHA_CPU_DISPATCH:
        key = "PUERTO_ESCUCHA_DISPATCH";
        break;
    case ESCUCHA_CPU_INTERRUPT:
        key = "PUERTO_ESCUCHA_INTERRUPT";
        break;
    case ESCUCHA_IO:
        key = "PUERTO_ESCUCHA_IO";
        break;
    }

    return config_get_string_value(kernel_config, key);
}

mem_address get_mem_adress()
{
    mem_address mem_addr;

    mem_addr.ip = config_get_string_value(kernel_config, "IP_MEMORIA");
    mem_addr.puerto = config_get_string_value(kernel_config, "PUERTO_MEMORIA");

    return mem_addr;
}

algoritmo_planificacion get_alg_plani_corto_plazo()
{
    char *algoritmo = config_get_string_value(kernel_config, "ALGORITMO_PLANIFICACION");

    if (strcmp(algoritmo, "FIFO") == 0)
        return FIFO;

    if (strcmp(algoritmo, "SJF") == 0)
        return SJF;

    if (strcmp(algoritmo, "SRT") == 0)
        return SRT;

    return -1;
}

// TODO: Implementar
// Deberían ser únicamente FIFO y SPF
// Esta pendiente de implementación dado que no se especifica
// la clave de la configuración en el TP.
algoritmo_planificacion get_alg_plani_largo_plazo()
{
    return -1;
}

u_int32_t get_tiempo_suspension()
{
    return config_get_int_value(kernel_config, "TIEMPO_SUSPENSION");
}

t_log_level get_log_level()
{
    char *log_level_str = config_get_string_value(kernel_config, "LOG_LEVEL");
    return  log_level_from_string(log_level_str);
}
