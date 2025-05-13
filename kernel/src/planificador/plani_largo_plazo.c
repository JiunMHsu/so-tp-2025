#include "plani_largo_plazo.h"

// TODO: implementar planificador de largo plazo

// void crear_procesos() 
// {
//     sem_wait(&iniciar_planificacion);
    
//     while(1)
//     {
//        //no entiendo como le llega el procesos
//         queue_push(NEW, proceso_nuevo);

//         if(queue_size(NEW) == 1) 
//         {
//             esperar_solicitud(proceso_nuevo);
//         }
//         else
//         {
//             //TODO: agregar a redy segun el algoritmo
//         }
//     }


// }


// void esperar_solicitud(Proceso* proceso_nuevo) 
// {
//     while (1) 
//     {
//         int32_t solicitud = solicitar_creacion_proceso(proceso_nuevo->pid, proceso_nuevo->tamanio, );
//         if (solicitud == 1) 
//         {
//             queue_pop(NEW);
//             queue_push(READY, proceso_nuevo);
//             break;
//         } 
//         else 
//         {
//             while (!finalizo_otro_proceso()); //TODO
//         }
//     }
// }
