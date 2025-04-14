#include "config.h"

t_config *cpu_config;

void iniciar_config()
{
    cpu_config = config_create(CONFIG_FILE);

    if (cpu_config == NULL)
    {
        perror("No se encotro el archivo de configuracion del cpu.");
        abort();
    }
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