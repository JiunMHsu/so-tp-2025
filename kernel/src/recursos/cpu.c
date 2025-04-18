#include "cpu.h"

// TODO: reemplazar por colecciones thread-safe (listas y colas con mutex)
t_list *cpus;
t_queue *desalojados;

static t_cpu *crear_cpu(char *id);
static t_cpu *buscar_por_id(char *id);
static t_cpu *buscar_por_pid(u_int32_t pid);
static t_cpu *buscar_libre(void);

static void *ejecutar(void *contexto);

void inicializar_cpu()
{
}

void *manejar_conexion_dispatch(void *fd_cpu)
{
    int32_t fd_dispatch = *((int32_t *)fd_cpu);
    free(fd_cpu);

    if (recibir_cliente(fd_dispatch) != CPU)
    {
        log_mensaje_error("Error cliente inválido");
        return NULL;
    }

    char *id_cpu = recibir_mensaje(fd_dispatch);
    t_cpu *cpu = buscar_por_id(id_cpu);
    if (cpu != NULL)
    {
        cpu->fd_dispatch = fd_dispatch;
        return NULL;
    }

    cpu = crear_cpu(id_cpu);
    cpu->fd_dispatch = fd_dispatch;
    list_add(cpus, cpu);

    return NULL;
}

void *manejar_conexion_interrupt(void *fd_cpu)
{
    int32_t fd_interrupt = *((int32_t *)fd_cpu);
    free(fd_cpu);

    if (recibir_cliente(fd_interrupt) != CPU)
    {
        log_mensaje_error("Error cliente inválido");
        return NULL;
    }

    char *id_cpu = recibir_mensaje(fd_interrupt);
    t_cpu *cpu = buscar_por_id(id_cpu);
    if (cpu != NULL)
    {
        cpu->fd_interrupt = fd_interrupt;
        return NULL;
    }

    cpu = crear_cpu(id_cpu);
    cpu->fd_interrupt = fd_interrupt;
    list_add(cpus, cpu);

    return NULL;
}

int8_t ejecutar_proceso(u_int32_t pid, u_int32_t program_counter)
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
    pthread_create(&hilo_ejecucion, NULL, &ejecutar, &args);
    pthread_detach(hilo_ejecucion);

    return 0;
}

static void *ejecutar(void *_args)
{
    args_ejecucion args = *((args_ejecucion *)_args);
    u_int32_t pid = args.pid;
    u_int32_t program_counter = args.program_counter;
    int32_t fd_dispatch = args.fd_dispatch;

    // empaquetar pid y program_counter
    // enviar
    // esperar y escuchar respuesta

    // armar t_desalojo
    // push a desalojados

    return NULL;
}

// TODO
void enviar_interrupcion(u_int32_t pid)
{
}

// TODO
t_desalojo *get_desalojo(void)
{
    return NULL;
}

// TODO
void destruir_desalojo(t_desalojo *desalojado)
{
}

static t_cpu *crear_cpu(char *id)
{
    t_cpu *cpu = malloc(sizeof(t_cpu));
    cpu->fd_dispatch = -1;
    cpu->fd_interrupt = -1;
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