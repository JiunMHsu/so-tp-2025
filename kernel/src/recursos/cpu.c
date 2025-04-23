#include "cpu.h"

// TODO: reemplazar por colecciones thread-safe (listas y colas con mutex)
t_list *cpus;
t_queue *desalojados;

static t_cpu *crear_cpu(char *id, int32_t fd_dispatch, int32_t fd_interrupt);
static t_cpu *buscar_por_id(char *id);
static t_cpu *buscar_por_pid(u_int32_t pid);
static t_cpu *buscar_libre(void);

static void *_ejecutar(void *contexto);

void inicializar_cpu()
{
}

void conectar_cpu(char *id_cpu, int32_t fd_dispatch, int32_t fd_interrupt)
{
    t_cpu *cpu = buscar_por_id(id_cpu);
    if (cpu != NULL)
    {
        log_mensaje_error("Error id CPU existente");

        free(id_cpu);
        cerrar_conexion(fd_dispatch);
        cerrar_conexion(fd_interrupt);
        return;
    }

    cpu = crear_cpu(id_cpu, fd_dispatch, fd_interrupt);
    list_add(cpus, cpu);
}

// La dinámica de ejecución no es óptima, ya que se crean y se destruyen hilos
// cada vez que pone un proceso en ejecución.
//
// Se podría crear una única rutina por CPU (hilo por CPU) y sincronizar por semáforos.
// La rutina se crearía en `conectar_cpu`, y cada una esperaría por procesos a ejecutar.
int8_t ejecutar(u_int32_t pid, u_int32_t program_counter)
{
    t_cpu *cpu_libre = buscar_libre();
    if (cpu_libre == NULL) // no debería suceder nunca
    {
        log_mensaje_error("No hay CPUs libres");
        return -1;
    }

    cpu_libre->executing = pid;

    args_ejecucion args;
    args.pid = pid;
    args.program_counter = program_counter;
    args.fd_dispatch = cpu_libre->fd_dispatch;

    pthread_t hilo_ejecucion;
    pthread_create(&hilo_ejecucion, NULL, &_ejecutar, &args);
    pthread_detach(hilo_ejecucion);

    return 0;
}

// TODO: implementar _ejecutar
static void *_ejecutar(void *_args)
{
    args_ejecucion args = *((args_ejecucion *)_args);
    u_int32_t pid = args.pid;
    u_int32_t program_counter = args.program_counter;
    int32_t fd_dispatch = args.fd_dispatch;

    t_peticion_ejecucion *peticion = crear_peticion_ejecucion(pid, program_counter);
    enviar_peticion_ejecucion(fd_dispatch, peticion);
    destruir_peticion_ejecucion(peticion);

    // esperar y escuchar desalojo, bloqueante
    // push a desalojados

    return NULL;
}

// TODO: implementar enviar_interrupcion
void enviar_interrupcion(u_int32_t pid)
{
}

// TODO: implementar get_desalojo
t_desalojo *get_desalojo(void)
{
    return NULL;
}

static t_cpu *crear_cpu(char *id, int32_t fd_dispatch, int32_t fd_interrupt)
{
    t_cpu *cpu = malloc(sizeof(t_cpu));
    cpu->fd_dispatch = fd_dispatch;
    cpu->fd_interrupt = fd_interrupt;
    cpu->id = strdup(id);
    cpu->executing = -1;

    return cpu;
}

static t_cpu *buscar_por_id(char *id)
{
    bool _tiene_id(void *_cpu)
    {
        t_cpu *cpu = (t_cpu *)_cpu;
        return strcmp(cpu->id, id) == 0;
    };

    return (t_cpu *)list_find(cpus, &_tiene_id);
}

static t_cpu *buscar_por_pid(u_int32_t pid)
{
    bool _tiene_pid(void *_cpu)
    {
        t_cpu *cpu = (t_cpu *)_cpu;
        return cpu->executing == pid;
    };

    return (t_cpu *)list_find(cpus, &_tiene_pid);
}

static t_cpu *buscar_libre(void)
{
    bool _esta_libre(void *_cpu)
    {
        t_cpu *cpu = (t_cpu *)_cpu;
        return cpu->executing == -1;
    };

    return (t_cpu *)list_find(cpus, &_esta_libre);
}
