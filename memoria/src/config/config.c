#include "config.h"

t_config *memoria_config;

void iniciar_config()
{
    memoria_config = config_create(CONFIG_FILE);
}

void destruir_config()
{
    config_destroy(memoria_config);
}

char *get_puerto_escucha()
{
    return config_get_string_value(memoria_config, "PUERTO_ESCUCHA");
}

t_log_level get_log_level()
{
    char* log_level_str = config_get_string_value(memoria_config, "LOG_LEVEL");
    return log_level_from_string(log_level_str);
}