#include "logger.h"

t_log *kernel_logger;

static void *u_int64_t_sum(void *a, void *b);

void iniciar_logger(t_log_level log_level)
{
    kernel_logger = log_create(LOG_FILE, "Kernel", true, log_level);
}

void destruir_logger(void)
{
    log_destroy(kernel_logger);
}

void log_evento(char *evento)
{
    log_info(kernel_logger, "%s", evento);
}

void log_mensaje_error(char *error)
{
    log_error(kernel_logger, "%s", error);
}

void log_syscall_recibida(u_int32_t pid, char *syscall)
{
    log_info(kernel_logger, "## (%d) - Solicitó syscall: %s", pid, syscall);
}

void log_creacion_proceso(u_int32_t pid)
{
    log_info(kernel_logger, "## (%d) Se crea el proceso - Estado: NEW", pid);
}

void log_cambio_de_estado(u_int32_t pid, t_state anterior, t_state actual)
{
    char *anterior_str = get_estado_string(anterior);
    char *actual_str = get_estado_string(actual);
    log_info(kernel_logger, "## (%d) Pasa del estado %s al estado %s", pid, anterior_str, actual_str);
}

void log_motivo_bloqueo(u_int32_t pid, char *dispositivo_io)
{
    log_info(kernel_logger, "## (%d) - Bloqueado por IO: %s", pid, dispositivo_io);
}

void log_fin_de_io(u_int32_t pid)
{
    log_info(kernel_logger, "## (%d) finalizó IO y pasa a READY", pid);
}

void log_desalojo_srt(u_int32_t pid)
{
    log_info(kernel_logger, "## (%d) - Desalojado por fin de SRT", pid);
}

void log_finalizacion_proceso(u_int32_t pid)
{
    log_info(kernel_logger, "## (%d) - Finaliza el proceso", pid);
}

// TODO: Revisar si la salida es correcta, y si hay memory leaks
void log_metricas_proceso(u_int32_t pid, t_dictionary *metricas_estado, t_dictionary *metricas_tiempo)
{
    int8_t cantidad_estados = 7;
    char *keys[7] = {
        get_estado_string(NEW),
        get_estado_string(READY),
        get_estado_string(EXEC),
        get_estado_string(BLOCKED),
        get_estado_string(SUSPENDED_BLOCKED),
        get_estado_string(SUSPENDED_READY),
        get_estado_string(EXIT),
    };

    char *metricas_str = string_new();
    for (int i = 0; i < cantidad_estados; i++)
    {
        char *key = keys[i];
        u_int32_t *count = (u_int32_t *)dictionary_get(metricas_estado, key);
        t_list *time_list = list_duplicate((t_list *)dictionary_get(metricas_tiempo, key));

        // Si no me equivoco, después del fold, el primer elemento de la lista queda modificado
        // y el puntero retornado es el primero de la lista, es decir, si libero la lista, también libero el retornado.
        // revisar si es así
        u_int64_t *time = (u_int64_t *)list_fold1(time_list, &u_int64_t_sum);

        char *count_str = string_itoa(*count);
        char *time_str = string_itoa(*time);

        string_append_with_format(&metricas_str, "%s (%s) (%s)", key, count_str, time_str);

        if (i < cantidad_estados - 1)
            string_append_with_format(&metricas_str, ", ");

        list_destroy(time_list);
        free(count_str);
        free(time_str);
    }

    log_info(kernel_logger, "## (%d) - Métricas de estado: %s", pid, metricas_str);
    free(metricas_str);
}

static void *u_int64_t_sum(void *a, void *b)
{
    *(u_int64_t *)a += *(u_int64_t *)b;
    return a;
}
