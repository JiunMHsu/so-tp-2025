#include "cpu.h"

t_mutex_list *cpus;

sem_t *hay_cpu_libre;
sem_t *hay_finalizado;
t_mutex_queue *finalizados;

static void *_ejecutar(void *_cpu);

static t_cpu *crear_cpu(char *id, int32_t fd_dispatch, int32_t fd_interrupt);
static t_cpu *buscar_por_id(char *id);
static t_cpu *buscar_por_pid(u_int32_t pid);
static t_cpu *buscar_libre(void);

static t_fin_de_ejecucion *crear_fin_de_ejecucion(t_pcb *proceso, t_desalojo *desalojado);

void inicializar_cpu()
{
    cpus = mlist_create();
    finalizados = mqueue_create();

    hay_cpu_libre = malloc(sizeof(sem_t));
    sem_init(hay_cpu_libre, 0, 0);
    hay_finalizado = malloc(sizeof(sem_t));
    sem_init(hay_finalizado, 0, 0);
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

    pthread_t hilo_ejecucion;
    pthread_create(&hilo_ejecucion, NULL, &_ejecutar, cpu);
    pthread_detach(hilo_ejecucion);

    mlist_add(cpus, cpu);
    sem_post(hay_cpu_libre);
}

void ejecutar(t_pcb *proceso)
{
    sem_wait(hay_cpu_libre);

    t_cpu *cpu_libre = buscar_libre();

    pthread_mutex_lock(&(cpu_libre->mutex_proceso));
    cpu_libre->proceso = proceso;
    pthread_mutex_unlock(&(cpu_libre->mutex_proceso));
    sem_post(cpu_libre->hay_proceso);
}

static void *_ejecutar(void *_cpu)
{
    t_cpu *cpu = (t_cpu *)_cpu;
    int32_t fd_dispatch = cpu->fd_dispatch;

    while (1)
    {
        sem_wait(cpu->hay_proceso);

        pthread_mutex_lock(&(cpu->mutex_proceso));
        u_int32_t pid = cpu->proceso->pid;
        u_int32_t program_counter = cpu->proceso->program_counter;

        t_peticion_ejecucion *peticion = crear_peticion_ejecucion(pid, program_counter);
        enviar_peticion_ejecucion(fd_dispatch, peticion);

        t_desalojo *desalojado = recibir_desalojo(fd_dispatch);
        t_fin_de_ejecucion *finalizado = crear_fin_de_ejecucion(cpu->proceso,
                                                                desalojado->motivo,
                                                                desalojado->syscall);
        mqueue_push(finalizados, finalizado);
        sem_post(hay_finalizado);

        cpu->proceso = NULL;
        pthread_mutex_unlock(&(cpu->mutex_proceso));

        destruir_peticion_ejecucion(peticion);
        destruir_desalojo(desalojado);

        sem_post(hay_cpu_libre);
    }

    return NULL;
}

void enviar_interrupcion(u_int32_t pid)
{
    t_cpu *cpu = buscar_por_pid(pid);
    if (cpu == NULL)
    {
        log_mensaje_error("No se encontró la CPU que ejecuta el proceso");
        return;
    }

    enviar_senial(1, cpu->fd_interrupt);
}

t_fin_de_ejecucion *get_fin_de_ejecucion()
{
    sem_wait(hay_finalizado);
    return (t_fin_de_ejecucion *)mqueue_pop(finalizados);
}

static t_cpu *crear_cpu(char *id, int32_t fd_dispatch, int32_t fd_interrupt)
{
    t_cpu *cpu = malloc(sizeof(t_cpu));
    cpu->id = strdup(id);
    cpu->fd_dispatch = fd_dispatch;
    cpu->fd_interrupt = fd_interrupt;
    cpu->proceso = NULL;
    pthread_mutex_init(&(cpu->mutex_proceso), NULL);
    sem_init(cpu->hay_proceso, 0, 0);

    return cpu;
}

static t_cpu *buscar_por_id(char *id)
{
    int32_t _tiene_id(void *_cpu)
    {
        t_cpu *cpu = (t_cpu *)_cpu;
        return strcmp(cpu->id, id) == 0;
    };

    return (t_cpu *)mlist_find(cpus, &_tiene_id);
}

static t_cpu *buscar_por_pid(u_int32_t pid)
{
    int32_t _tiene_pid(void *_cpu)
    {
        t_cpu *cpu = (t_cpu *)_cpu;

        pthread_mutex_lock(&(cpu->mutex_proceso));
        if (cpu->proceso == NULL)
        {
            pthread_mutex_unlock(&(cpu->mutex_proceso));
            return 0;
        }

        int8_t ret = cpu->proceso->pid == pid;
        pthread_mutex_unlock(&(cpu->mutex_proceso));

        return ret;
    };

    return (t_cpu *)mlist_find(cpus, &_tiene_pid);
}

static t_cpu *buscar_libre(void)
{
    int32_t _esta_libre(void *_cpu)
    {
        t_cpu *cpu = (t_cpu *)_cpu;

        pthread_mutex_lock(&(cpu->mutex_proceso));
        int8_t ret = cpu->proceso == NULL;
        pthread_mutex_unlock(&(cpu->mutex_proceso));

        return ret;
    };

    return (t_cpu *)mlist_find(cpus, &_esta_libre);
}

static t_fin_de_ejecucion *crear_fin_de_ejecucion(t_pcb *proceso, t_desalojo *desalojado)
{
    t_fin_de_ejecucion *fin_de_ejecucion = malloc(sizeof(t_fin_de_ejecucion));

    fin_de_ejecucion->proceso = proceso;
    fin_de_ejecucion->proceso->program_counter = desalojado->program_counter;

    fin_de_ejecucion->motivo = desalojado->motivo;
    fin_de_ejecucion->syscall = strdup(desalojado->syscall);

    return fin_de_ejecucion;
}

void destruir_fin_de_ejecucion(t_fin_de_ejecucion *fin_de_ejecucion)
{
    if (fin_de_ejecucion == NULL)
        return;

    free(fin_de_ejecucion->syscall);
    free(fin_de_ejecucion);
    fin_de_ejecucion = NULL;
}
