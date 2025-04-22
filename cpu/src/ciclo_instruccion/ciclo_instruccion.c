#include "ciclo_instruccion.h"

void ejecutar_ciclo_instruccion(u_int32_t pid, u_int32_t pc) {}

// fetch (llamada a memoria)
// decode (llamada a funciones de instrucciones)
// execute (incrementar pc y ejecutar funcion instruccion)
// check desalojo (check syscall, mas prioridad que interrupciones)
// check interrupt (pregunta a interrupciones)
// desalojar si hay syscall o si hay interrupcion
