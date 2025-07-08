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

        u_int8_t resultado;

        switch (paquete->operacion)
        {
        case INICIAR_PROCESO:
            resultado = crear_proceso(paquete->pid,
                                      paquete->tamanio,
                                      paquete->path);
            enviar_senial(resultado, fd_kernel);
            break;

        case FINALIZAR_PROCESO:
            resultado = finalizar_proceso(paquete->pid);
            enviar_senial(resultado, fd_kernel);
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

        t_list *direcciones_fisicas;

        switch (peticion->operacion)
        {
        case FETCH_INSTRUCCION:
            char *instruccion = obtener_instruccion(peticion->pid, peticion->program_counter);

            log_obtencion_instruccion(peticion->pid, peticion->program_counter, instruccion);
            enviar_mensaje(instruccion, fd_cpu);
            free(instruccion); // si obtener instruccion no hiciera strdup, no haría falta
            break;
        
        case OBTENER_MARCO:
                u_int_32 *entradas_por_nivel = convertir_a_array_entradas_por_nivel(peticion->entradas_por_nivel);
                int32_t marco = obtener_marco(peticion->pid, peticion->entradas_por_nivel);

                free(entradas_por_nivel);
            // enviar

            break;

        case LEER:
            direcciones_fisicas = convertir_a_lista_de_direcciones_fisicas(peticion->direcciones_fisicas);
            void* lectura = leer_memoria_usuario(peticion->pid, direcciones_fisicas, peticion->tamanio_buffer);
            
            log_acceso_espacio_usuario(peticion->pid, LECTURA, direcciones_fisicas, peticion->tamanio_buffer);
            //enviar
            list_destroy_and_destroy_elements(direcciones_fisicas, &free);
            break;

        case ESCRIBIR:
            u_int8_t escritura = escribir_memoria_usuario(peticion->pid, direcciones_fisicas, peticion->buffer, peticion->tamanio_buffer);

            log_acceso_espacio_usuario(peticion->pid, ESCRITURA, direcciones_fisicas, peticion->tamanio_buffer);
            // enviar
            free(peticion->buffer);
            list_destroy_and_destroy_elements(direcciones_fisicas, &free);
            break;

        default:
            log_mensaje_error("Operacion no soportada.");
            break;
        }

        destruir_peticion_cpu(peticion);
    }

    return NULL;
}
