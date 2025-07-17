#include <utils/io/peticion_io.h>
#include <utils/io/motivo_fin_io.h>

#include "logger/logger.h"
#include "config/config.h"
#include "conexion/kernel.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Debes ingresarlo de la siguiente manera: %s <nombre_interfaz>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *nombre_interfaz = argv[1];

    iniciar_config();
    iniciar_logger(get_log_level());

    int32_t fd_kernel = conectar_con_kernel(nombre_interfaz);
    if (fd_kernel == -1)
        return EXIT_FAILURE;

    while (1)
    {
        t_peticion_io *peticion = recibir_peticion_io(fd_kernel);
        if (peticion == NULL)
        {
            cerrar_conexion(fd_kernel);
            log_mensaje_error("Error al recibir la petición de IO");
            return EXIT_FAILURE;
        }

        log_inicio_io(peticion->pid, peticion->tiempo);
        usleep(peticion->tiempo * 1000);
        log_finalizacion_io(peticion->pid);
        destruir_peticion_io(peticion);

        enviar_motivo_fin_io(fd_kernel, EXECUTED);
    }

    return EXIT_SUCCESS;
}
