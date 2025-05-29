#include <stdlib.h>

#include "config/config.h"
#include "logger/logger.h"
#include "conexion/kernel.h"
#include "conexion/memoria.h"
#include "interrupcion/interrupcion.h"
#include "instrucciones/instrucciones.h"
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

    if (conectar_kernel(id_cpu))
    {
        cerrar_conexion_kernel();
        log_mensaje_error("No se pudo establecer la conexion con el kernel.");
        return EXIT_FAILURE;
    }

    if (conectar_memoria())
    {
        cerrar_conexion_kernel();
        log_mensaje_error("No se pudo establecer la conexion con la memoria.");
        return EXIT_FAILURE;
    }

    inicializar_interrupcion();
    inicializar_instrucciones();

    while (1)
    {
        t_peticion_ejecucion *peticion_exec = recibir_peticion_ejecucion_kernel();
        if (peticion_exec == NULL)
        {
            log_mensaje_error("Error al recibir la petición de ejecución.");

            cerrar_conexion_kernel();
            cerrar_conexion_memoria();
            return EXIT_FAILURE;
        }

        resetear_interrupcion();
        fin_ejecucion fin_ejecucion = ejecutar_ciclo_instruccion(peticion_exec->pid,
                                                                 peticion_exec->program_counter);

        enviar_desalojo_kernel(peticion_exec->pid,
                               fin_ejecucion.program_counter,
                               fin_ejecucion.motivo,
                               fin_ejecucion.syscall);

        destruir_peticion_ejecucion(peticion_exec);

        if (fin_ejecucion.syscall != NULL)
            free(fin_ejecucion.syscall);
    }

    return EXIT_SUCCESS;
}
