#include "servidor.h"

int32_t fd_escucha;
sem_t fin_de_proceso;

static void *atender_kernel(void *fd_ptr);
static void *atender_cpu(void *fd_ptr);

void iniciar_servidor()
{
    char *puerto_escucha = get_puerto_escucha();
    fd_escucha = crear_servidor(puerto_escucha);

    while (1)
    {
        int32_t *fd_cliente = malloc(sizeof(int32_t));
        *fd_cliente = esperar_cliente(fd_escucha, NULL);

        int8_t modulo_cliente = recibir_cliente(*fd_cliente);
        switch (modulo_cliente)
        {
        case KERNEL:
            log_evento("Kernel conectado.");

            pthread_t atencion_kernel;
            pthread_create(&atencion_kernel, NULL, &atender_kernel, fd_cliente);
            pthread_detach(atencion_kernel);
            break;
        case CPU:
            log_evento("CPU conectado.");

            pthread_t atencion_cpu;
            pthread_create(&atencion_cpu, NULL, &atender_cpu, fd_cliente);
            pthread_detach(atencion_cpu);
            break;
        default:
            log_mensaje_error("Modulo desconocido.");
        }
    }
}

void finalizar_servidor(void)
{
    log_evento("Finalizando servidor...");
    cerrar_conexion(fd_escucha);
    log_evento("Servidor finalizado.");
}

void finalizar_servidor_por_sigint(int _)
{
    finalizar_servidor();
    exit(EXIT_SUCCESS);
}

static void *atender_kernel(void *fd_ptr)
{
    int32_t fd_kernel = *((int32_t *)fd_ptr);
    free(fd_ptr);

    while (1)
    {
        t_kernel_mem_req *paquete = recibir_kernel_mem_request(fd_kernel);

        if (paquete == NULL)
            return NULL;

        switch (paquete->operacion)
        {

        case INICIAR_PROCESO:
            iniciar_proceso(paquete->pid, paquete->path);
            int32_t valor_mock = 1; // Enviar valor entero mock (1 = OK)
            enviar_senial(valor_mock, fd_kernel);
            break;

        case FINALIZAR_PROCESO:
            finalizar_proceso(paquete->pid);
            int32_t valor_mock = 1;
            enviar_senial(valor_mock, fd_kernel);
            break;

        default:
            break;
        }

        destruir_kernel_mem_request(paquete);
    }

    close(fd_kernel);
    return NULL;
}

static void *atender_cpu(void *fd_ptr)
{
    int32_t fd_kernel = *((int32_t *)fd_ptr);
    free(fd_ptr);

    while (1)
    {
        // escuchar peticiones
    }

    return NULL;
}
