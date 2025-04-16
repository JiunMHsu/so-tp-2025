#include <utils/sockets/sockets.h>
#include <utils/protocol/protocol.h>
#include <commons/log.h>
#include <commons/config.h>

int32_t fd_kernel;

t_log* logger;
t_config* config;

char* IP_KERNELL;
uint16_t PUERTO_KERNELL;
t_log_level LOG_LEVEL;

t_log* iniciar_logger(void);
t_config* iniciar_config(void);
int8_t conectar_con_kernel(char *nombre_interfaz);

int main(int argc, char *argv[]) 
{
    if(argc != 2){
        fprintf(stderr, "Debes ingresarlo de la siguiente manera: %s <nombre_interfaz>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char* nombre_interfaz = argv[1];

    config = iniciar_config();
    
    IP_KERNELL = config_get_string_value(config, "IP_KERNELL");
    PUERTO_KERNELL = (uint16_t) config_get_int_value(config, "PUERTO_KERNELL");
    char* LOG_LEVEL_STR = config_get_string_value(config, "LOG_LEVEL");
    LOG_LEVEL = log_level_from_string(LOG_LEVEL_STR);

    logger = iniciar_logger();

    /* Loggeo el valor de config para confirmar los valores
    log_info(logger, "IP_KERNELL: %s", IP_KERNELL);
    log_info(logger, "PUERTO_KERNELL: %d", PUERTO_KERNELL);
    log_info(logger, "LOG_LEVEL: %s", LOG_LEVEL_STR); */

    // Creo conexion hacia kernell (aca quede)
    int8_t resultado = conectar_con_kernel(nombre_interfaz);
    if (resultado == -1)
        return EXIT_FAILURE;

    while (1) // Es una espera activa? Hay que hacer uso del semaforo?
    {
        char *mensaje = recibir_mensaje(fd_kernel);
        if (mensaje == NULL)
        {
            printf("El servidor se desconectó. Cerrando conexión...\n");
            cerrar_conexion(fd_kernel);
            return EXIT_FAILURE;
        }

        printf("Mensaje recibido: %s\n", mensaje);
        free(mensaje);
    }

    return EXIT_SUCCESS;
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("io.log", "Modulo IO", 0, LOG_LEVEL);
	if(nuevo_logger == NULL){
		perror("Ocurrio un error. No se pudo crear el archivo");
		exit(EXIT_FAILURE);
	}

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config = config_create("io.config");
	if(nuevo_config == NULL){
		perror("Ocurrio un error. No se pudo encontrar el archivo en el path especificado");
		exit(EXIT_FAILURE);
	}

	return nuevo_config;
}

int8_t conectar_con_kernel(char *nombre_interfaz)
{
    fd_kernel = crear_conexion(IP_KERNELL, PUERTO_KERNELL);
    int32_t response = handshake(fd_kernel, IO);

    if (response == -1)
    {
        cerrar_conexion(fd_kernel);
        return -1;
    }
    enviar_mensaje(nombre_interfaz,fd_kernel);

    return 0;
}
