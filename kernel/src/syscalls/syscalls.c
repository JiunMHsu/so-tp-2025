#include "syscalls.h"

static void dump_memory(t_pcb *proceso);
static void *_dump_memory(void *_pid);
static void io(t_pcb *proceso, char *dispositivo, u_int32_t tiempo);
static void exit_proc(t_pcb *proceso);

void manejar_syscall(t_pcb *proceso, char *syscall)
{
    char **syscall_vec = string_split(syscall, " ");
    char *syscall_name = syscall_vec[0];
    log_syscall_recibida(proceso->pid, syscall_name);

    if (string_is_equal(syscall_name, "IO"))
    {
        char *dispositivo = syscall_vec[1];
        u_int32_t tiempo = atoi(syscall_vec[2]);

        io(proceso, dispositivo, tiempo);
    }

    if (string_is_equal(syscall_name, "EXIT"))
        exit_proc(proceso);

    if (string_is_equal(syscall_name, "DUMP_MEMORY"))
        dump_memory(proceso);

    string_array_destroy(syscall_vec); // TODO: ver si se debe destruir aca
}

u_int8_t is_init_proc(char *syscall)
{
    if (syscall == NULL)
        return 0;

    return string_starts_with(syscall, "INIT_PROC");
}

void init_proc(t_pcb *proceso, char *syscall)
{
    char **syscall_vec = string_split(syscall, " ");

    log_syscall_recibida(proceso->pid, syscall_vec[0]);

    char *pseudocodigo = remove_new_line(syscall_vec[1]);
    u_int32_t tamanio_proceso = atoi(syscall_vec[2]);

    insertar_proceso_nuevo(pseudocodigo, tamanio_proceso);
    string_array_destroy(syscall_vec);
}

static void dump_memory(t_pcb *proceso)
{
    insertar_en_blocked(proceso);

    pthread_t thread_dump;
    pthread_create(&thread_dump, NULL, &_dump_memory, proceso);
    pthread_detach(thread_dump);
}

static void *_dump_memory(void *_proceso)
{
    t_pcb *proceso = (t_pcb *)_proceso;

    int8_t respuesta = solicitar_dump_proceso(proceso->pid);
    u_int8_t fallo = respuesta ? 0 : 1;
    log_evento("a desbloquear por fin de dump");
    desbloquear_proceso(proceso, fallo);

    return NULL;
}

static void io(t_pcb *proceso, char *dispositivo, u_int32_t tiempo)
{
    int32_t intento_de_consumo = bloquear_para_io(dispositivo, proceso, tiempo);
    if (intento_de_consumo == -1)
    {
        insertar_en_exit(proceso);
        return;
    }

    insertar_en_blocked(proceso);
}

static void exit_proc(t_pcb *proceso)
{
    insertar_en_exit(proceso);
}
