#include "config.h"

t_config *cpu_config;

void iniciar_config()
{
    cpu_config = config_create(CONFIG_FILE);
}

void destruir_config()
{
    config_destroy(cpu_config);
}

kernel_address get_kernel_address()
{
    kernel_address kernel_addrs;

    kernel_addrs.ip = config_get_string_value(cpu_config, "IP_KERNEL");
    kernel_addrs.puerto_dispatch = config_get_string_value(cpu_config, "PUERTO_KERNEL_DISPATCH");
    kernel_addrs.puerto_interrupt = config_get_string_value(cpu_config, "PUERTO_KERNEL_INTERRUPT");

    return kernel_addrs;
}

mem_address get_memoria_address()
{
    mem_address memoria_addrs;

    memoria_addrs.ip = config_get_string_value(cpu_config, "IP_MEMORIA");
    memoria_addrs.puerto = config_get_string_value(cpu_config, "PUERTO_MEMORIA");

    return memoria_addrs;
}

t_log_level get_log_level()
{
    char *log_level_str = config_get_string_value(cpu_config, "LOG_LEVEL");

    return log_level_from_string(log_level_str);
}

u_int32_t get_entradas_tlb()
{
    return config_get_int_value(cpu_config, "ENTRADAS_TLB");
}

u_int32_t get_entradas_cache()
{
    return config_get_int_value(cpu_config, "ENTRADAS_CACHE");
}

algoritmo_sustitucion get_algoritmo_sustitucion_tlb()
{
    char *algoritmo = config_get_string_value(cpu_config, "REEMPLAZO_TLB");

    if (strcmp(algoritmo, "FIFO") == 0)
        return FIFO;

    return LRU;
}

algoritmo_sustitucion get_algoritmo_sustitucion_cache()
{
    char *algoritmo = config_get_string_value(cpu_config, "REEMPLAZO_CACHE");

    if (strcmp(algoritmo, "CLOCK") == 0)
        return CLOCK;

    return CLOCK_M;
}
