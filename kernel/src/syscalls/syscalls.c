#include "syscalls.h"

static void init_proc(char *pseudocodigo, u_int32_t tamanio_proceso);
static void dump_memory(t_pcb *proceso);
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
}

static void init_proc(char *pseudocodigo, u_int32_t tamanio_proceso)
{
    insertar_proceso_nuevo(pseudocodigo, tamanio_proceso);
}

// TODO: Implementar dump_memory
static void dump_memory(t_pcb *proceso)
{
    // hacer la peticion
    // bloquear con plani mediano plazo
}

// TODO: Implementar llamada a IO
static void io(t_pcb *proceso, char *dispositivo, u_int32_t tiempo)
{
    // hacer la peticion
    // bloquear con plani mediano plazo
}

static void exit_proc(t_pcb *proceso)
{
    insertar_en_exit(proceso);
}
