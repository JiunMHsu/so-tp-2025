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
    char *log_level_str = config_get_string_value(memoria_config, "LOG_LEVEL");
    return log_level_from_string(log_level_str);
}

int32_t get_tam_memoria()
{
    return config_get_int_value(memoria_config, "TAM_MEMORIA");
}

int32_t get_tam_pagina()
{
    return config_get_int_value(memoria_config, "TAM_PAGINA");
}

int32_t get_entradas_por_tabla()
{
    return config_get_int_value(memoria_config, "ENTRADAS_POR_TABLA");
}

int32_t get_cantidad_niveles()
{
    return config_get_int_value(memoria_config, "CANTIDAD_NIVELES");
}

int32_t get_retardo_memoria()
{
    return config_get_int_value(memoria_config, "RETARDO_MEMORIA");
}

char *get_path_swapfile()
{
    return config_get_string_value(memoria_config, "PATH_SWAPFILE");
}

int32_t get_retardo_swap()
{
    return config_get_int_value(memoria_config, "RETARDO_SWAP");
}

char *get_dump_path()
{
    return config_get_string_value(memoria_config, "DUMP_PATH");
}