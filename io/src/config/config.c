#include "config.h"

t_config *io_config;

void iniciar_config()
{
    io_config = config_create("io.config");
    if (io_config == NULL)
    {
        perror("Error al cargar el archivo de configuración");
        exit(EXIT_FAILURE);
    }
}

void destruir_config()
{
    config_destroy(io_config);
}

kernel_address get_kernel_address()
{
    kernel_address address;
    address.ip = config_get_string_value(io_config, "IP_KERNEL");
    address.puerto = config_get_string_value(io_config, "PUERTO_KERNEL");

    return address;
}

t_log_level get_log_level()
{
    char *log_level_str = config_get_string_value(io_config, "LOG_LEVEL");
    return log_level_from_string(log_level_str);
}