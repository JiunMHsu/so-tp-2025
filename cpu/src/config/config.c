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