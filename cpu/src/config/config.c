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

char *get_puerto_server(puerto_server puerto)
{
    char *key;
    switch (puerto)
    {
    case PUERTO_KERNEL_DISPATCH:
        key = "PUERTO_KERNEL_DISPATCH";
        break;
    case PUERTO_KERNEL_INTERRUPT:
        key = "PUERTO_KERNEL_INTERRUPT";
        break;
    case PUERTO_MEMORIA:
        key = "PUERTO_MEMORIA";
        break;
    }

    return config_get_string_value(cpu_config, key);
}

char *get_ip_server(ip_server ip)
{
    char *key;
    switch (ip)
    {
    case IP_KERNEL:
        key = "IP_KERNEL";
        break;
    case IP_MEMORIA:
        key = "IP_MEMORIA";
        break;
    }

    return config_get_string_value(cpu_config, key);
}

// TODO
kernel_address get_kernel_address()
{
}