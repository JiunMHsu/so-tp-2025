#include <stdlib.h>

#include "conexion/kernel.h"
#include "conexion/memoria.h"
#include "interrupcion/interrupcion.h"
#include "ciclo_instruccion/ciclo_instruccion.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Se debe especificar el id de la CPU \n");
        return EXIT_FAILURE;
    }

    char *id_cpu = argv[1];

    iniciar_config();
    // TODO: falta agregar id_cpu como parametro -> Requerimiento del tp, se debe poder identificar a cpu pertenece el logger
    iniciar_logger();

    t_kernel_sockets kernel_sockets = conectar_kernel(id_cpu);
    if (kernel_sockets.fd_dispatch == -1 || kernel_sockets.fd_interrupt == -1)
    {
        log_mensaje_error("No se pudo establecer la conexion con el kernel.");
        return EXIT_FAILURE;
    }

    if (conectar_memoria())
    {
        cerrar_conexion(kernel_sockets.fd_dispatch);
        cerrar_conexion(kernel_sockets.fd_interrupt);
        return EXIT_FAILURE;
    }

    inicializar_interrupcion(kernel_sockets.fd_interrupt);
    inicializar_instrucciones();

    while (1)
    {
        t_peticion_ejecucion *peticion = recibir_peticion_ejecucion(kernel_sockets.fd_dispatch);
        if (peticion == NULL)
        {
            log_mensaje_error("Error al recibir la peticion de ejecucion.");
            cerrar_conexion(kernel_sockets.fd_dispatch);
            cerrar_conexion(kernel_sockets.fd_interrupt);
            // cerrar_conexion_memoria();
            return EXIT_FAILURE;
        }

        fin_ejecucion fin_ejecucion = ejecutar_ciclo_instruccion(peticion->pid,
                                                                 peticion->program_counter);

        //  armar el desalojo
        //  enviar el desalojo
        t_desalojo *desalojo = crear_desalojo(peticion->pid, fin_ejecucion.program_counter, fin_ejecucion.motivo, fin_ejecucion.syscall);
        enviar_desalojo(kernel_sockets.fd_dispatch, desalojo);

        // destruir la peticion
        // destruir el desalojo
        // TODO: revisar destruir fin_ejecucion, es necesario?
        destruir_peticion_cpu(peticion);
        destruir_desalojo(desalojo);
    }

    return EXIT_SUCCESS;
}
