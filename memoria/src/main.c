#include "logger/logger.h"
#include "servidor/servidor.h"

int main(int argc, char *argv[])
{
    iniciar_config();
    
    iniciar_logger(get_log_level());

    iniciar_servidor();

    escuchar_conexiones(); 

    finalizar_servidor();

    return 0;
}

//void * se usa porque cuando crees el hilo lo va a necesitar asi
void escuchar_conexiones(void *fd_escucha) { 
    
    // agregar log 
    while (1) { //casteo fd_escucha a int32_t y despues le paso el valor de lo apuntado con el *
        esperar_cliente(*((int32_t*) fd_escucha), &atender_cliente);
    }
    
    return NULL;
}


void *aternder_cliente(void * fd_ptr) {
    int32_t fd_conexion = *((int32_t *)fd_ptr);
   
    uint32_t modulo_cliente = recibir_cliente(fd_conexion);

    switch (modulo_cliente)
    {
       case KERNEL:
            break;

        case CPU:
            break;

        default:
            break;
    }

}