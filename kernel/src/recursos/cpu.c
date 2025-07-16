#include "cpu.h"

static t_mutex_list *cpus;
static t_mutex_queue *cpus_libres;

static sem_t *hay_cpu_libre;
static sem_t *hay_desalojado;
static t_mutex_queue *desalojados;

static void *_ejecutar(void *_cpu);

static t_cpu *crear_cpu(char *id, int32_t fd_dispatch, int32_t fd_interrupt);
static t_cpu *buscar_por_id(char *id);
static t_cpu *buscar_por_pid(u_int32_t pid);

void inicializar_cpu()
{
    cpus = mlist_create();
    cpus_libres = mqueue_create();
    desalojados = mqueue_create();

    hay_cpu_libre = malloc(sizeof(sem_t));
    sem_init(hay_cpu_libre, 0, 0);
    hay_desalojado = malloc(sizeof(sem_t));
    sem_init(hay_desalojado, 0, 0);
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
    mqueue_push(cpus_libres, cpu);
    sem_post(hay_cpu_libre);
}

void ejecutar(t_pcb *proceso)
{
    sem_wait(hay_cpu_libre);
    t_cpu *cpu_libre = mqueue_pop(cpus_libres);

    pthread_mutex_lock(&(cpu_libre->mutex_proceso));
    cpu_libre->proceso = proceso;
    pthread_mutex_unlock(&(cpu_libre->mutex_proceso));
    sem_post(cpu_libre->hay_proceso);
}

int8_t hay_cpu()
{
    int32_t ret = 0;
    sem_getvalue(hay_cpu_libre, &ret);
    return ret > 0;
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
        pthread_mutex_unlock(&(cpu->mutex_proceso));

        t_desalojo *desalojado = NULL;

        while (1)
        {
            destruir_desalojo(desalojado); // NULL safe

            t_peticion_ejecucion *peticion = crear_peticion_ejecucion(pid, program_counter);
            enviar_peticion_ejecucion(fd_dispatch, peticion);
            destruir_peticion_ejecucion(peticion);

            desalojado = recibir_desalojo(fd_dispatch);
            if (desalojado == NULL)
            {
                log_mensaje_error("Error al recibir desalojo");
                exit(EXIT_FAILURE);
            }

            if (is_init_proc(desalojado->syscall))
            {
                init_proc(cpu->proceso, desalojado->syscall);
                continue;
            }

            break;
        }

        // si se recibe un desalojo, se marca inmediatamente el proceso como NULL
        // es para evitar que se envíe una interrupción a la CPU.
        // igualmente va a pasar que se envíe interrupciones cuando el proceso ya está desalojado,
        // es responsabilidad de la CPU ignorar esas señales.
        pthread_mutex_lock(&(cpu->mutex_proceso));
        cpu->proceso = NULL;
        pthread_mutex_unlock(&(cpu->mutex_proceso));

        mqueue_push(desalojados, desalojado);
        sem_post(hay_desalojado);

        mqueue_push(cpus_libres, cpu);
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

t_desalojo *get_desalojado()
{
    sem_wait(hay_desalojado);
    return (t_desalojo *)mqueue_pop(desalojados);
}

static t_cpu *crear_cpu(char *id, int32_t fd_dispatch, int32_t fd_interrupt)
{
    t_cpu *cpu = malloc(sizeof(t_cpu));
    cpu->id = strdup(id);
    cpu->fd_dispatch = fd_dispatch;
    cpu->fd_interrupt = fd_interrupt;
    cpu->proceso = NULL;
    pthread_mutex_init(&(cpu->mutex_proceso), NULL);
    cpu->hay_proceso = malloc(sizeof(sem_t));
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
