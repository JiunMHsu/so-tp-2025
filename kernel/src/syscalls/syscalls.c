#include "syscalls.h"

static void init_proc(char *pseudocodigo, u_int32_t tamanio_proceso);
static void dump_memory(t_pcb *proceso);
static void *_dump_memory(void *_pid);
static void io(t_pcb *proceso, char *dispositivo, u_int32_t tiempo);
static void exit_proc(t_pcb *proceso);

void manejar_syscall(t_pcb *proceso, char *syscall)
{
    char **syscall_vec = string_split(syscall, " ");
    char *syscall_name = syscall_vec[0];

    if (string_is_equal(syscall_name, "INIT_PROC"))
    {
        char *pseudocodigo = remove_new_line(syscall_vec[1]);
        u_int32_t tamanio_proceso = atoi(syscall_vec[2]);

        init_proc(pseudocodigo, tamanio_proceso);
    }

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

    string_array_destroy(syscall_vec); // TODO: averiguar si se debe destruir aca
}

static void init_proc(char *pseudocodigo, u_int32_t tamanio_proceso)
{
    insertar_proceso_nuevo(pseudocodigo, tamanio_proceso);
}

// TODO: Implementar dump_memory
static void dump_memory(t_pcb *proceso)
{
    // hacer la peticion
    u_int32_t *pid = malloc(sizeof(u_int32_t));
    *pid = proceso->pid;

    insertar_en_blocked(proceso);

    pthread_t thread_dump;
    pthread_create(&thread_dump, NULL, &_dump_memory, pid);
    pthread_detach(thread_dump);
}

static void *_dump_memory(void *_pid)
{
    u_int32_t pid = *(u_int32_t *)_pid;
    free(_pid);

    int8_t respuesta = solicitar_dump_proceso(pid);
    // de alguna forma notificar al plani mediano plazo que ya termino el dump

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
