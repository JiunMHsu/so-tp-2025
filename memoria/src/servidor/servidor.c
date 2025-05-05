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

    while(1) 
    {
        t_list* elementos = recibir_paquete(fd_kernel);

        if(elementos == NULL) {
            //TODO ver que hacer cuando el elemento enviado esta vacio
        }

        // procesar
        // Mock: asumimos que solo se recibe el tamaño (int)
        int32_t* tamanio = list_get(elementos,0);
        int32_t pid = list_size(procesos_mock);
        
        // Creo un nuevo nodo para la lista mockeada 
        t_proceso_mock* nuevo = malloc(sizeof(t_proceso_mock));
        nuevo->pid = pid;
        nuevo->size = *tamanio;
        list_add(procesos_mock, nuevo);

        log_creacion_proceso(pid, *tamanio);

        // responder
        int32_t valor_mock = 1;   // Enviar valor entero mock (1 = OK)
        t_packet* respuesta = crear_paquete();
        agregar_a_paquete(respuesta, &valor_mock, sizeof(int32_t));
        enviar_paquete(respuesta, fd_kernel);
        eliminar_paquete(respuesta);

        // desconectar
        list_destroy_and_destroy_elements(elementos, 1, free);
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
