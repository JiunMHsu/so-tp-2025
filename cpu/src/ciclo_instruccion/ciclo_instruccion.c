#include "ciclo_instruccion.h"

fin_ejecucion ejecutar_ciclo_instruccion(u_int32_t pid, u_int32_t pc)
{
    while (1)
    {
        // fetch (llamada a memoria)
        // decode (llamada a funciones de instrucciones)
        // execute (incrementar pc y ejecutar funcion instruccion)
        // check desalojo (check syscall, mas prioridad que interrupciones)
        // check interrupt (pregunta a interrupciones)

        // desalojar si hay syscall o si hay interrupcion
    }

    return (fin_ejecucion){0, 0, NULL}; // TODO: devolver el motivo de desalojo
}
