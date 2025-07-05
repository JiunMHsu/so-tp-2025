#include "servidor.h"

int32_t fd_escucha;

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
            crear_proceso(paquete->pid, paquete->path);
            int32_t valor_mock = 500;
            enviar_senial(valor_mock, fd_kernel);
            break;

        case FINALIZAR_PROCESO:
            finalizar_proceso(paquete->pid);
            enviar_senial(1, fd_kernel);
            break;

        case DUMP_PROCESO:
            break;

        case SWAP_OUT:
            break;

        case SWAP_IN:
            break;

        default: // no debería ocurrir nunca
            break;
        }

        destruir_kernel_mem_request(paquete);
    }

    close(fd_kernel);
    return NULL;
}

static void *atender_cpu(void *fd_ptr)
{
    int32_t fd_cpu = *((int32_t *)fd_ptr);
    free(fd_ptr);

    while (1)
    {
        t_peticion_cpu *peticion = recibir_peticion_cpu(fd_cpu);

        if (peticion == NULL)
        {
            log_mensaje_error("Error al recibir la peticion de ejecucion. Cerrando conexion con CPU...");
            cerrar_conexion(fd_cpu);
            return NULL;
        }

        switch (peticion->operacion)
        {
        case FETCH_INSTRUCCION:
            char *instruccion = obtener_instruccion(peticion->pid, peticion->program_counter);

            log_obtencion_instruccion(peticion->pid, peticion->program_counter, instruccion);
            enviar_mensaje(instruccion, fd_cpu);
            free(instruccion); // si obtener instruccion no hiciera strdup, no haría falta
            break;

        default:
            log_mensaje_error("Operacion no soportada.");
            break;
        }

        destruir_peticion_cpu(peticion);
    }

    return NULL;
}
