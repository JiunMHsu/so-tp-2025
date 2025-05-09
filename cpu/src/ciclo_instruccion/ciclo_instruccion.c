#include "ciclo_instruccion.h"

fin_ejecucion ejecutar_ciclo_instruccion(u_int32_t pid, u_int32_t program_counter)
{
    while (1)
    {
        // fetch (llamada a memoria)
        // decode (llamada a funciones de instrucciones)
        // execute (incrementar pc y ejecutar funcion instruccion)
        // check desalojo (check syscall, mas prioridad que interrupciones)
        // check interrupt (pregunta a interrupciones)

        // desalojar si hay syscall o si hay interrupcion

        // por ahora
        char *instruccion_a_ejecutar = fetch(pid, program_counter);

        instruccion_ejecutable instruccion = decode(instruccion_a_ejecutar); //TODO: tendria que ser un puntero a instruccion_ejecutable?

        execute(instruccion, &program_counter);
    }

    return (fin_ejecucion){0, 0, NULL}; // TODO: devolver el motivo de desalojo
}

// Recibo peticion de ejecucion desde kernell

char *fetch(u_int32_t pid, u_int32_t program_counter)
{
    // creo una mem_request -> mando pid y pc
    // log fectch instruccion -> muestra el pc
    // la instruccion se recibe como un mensaje -> memoria solo envia un texto plano, cpu decodifica

    // TODO: puede ser que esto quedase mejor en una funcion enviar_mem_request(pid, pc)
    //--
    t_packet paquete = crear_paquete();

    agregar_a_paquete(paquete, pid, sizeof(u_int32_t));
    agregar_a_paquete(paquete, program_counter, sizeof(u_int32_t));

    enviar_paquete(paquete, fd_memoria);
    //--

    char *instruccion_recibida = recibir_mensaje(fd_memoria);

    return instruccion_recibida;
}

instruccion_ejecutable decode(char *instruccion_recibida)
{
    // obtengo la funcion que le corresponde a la instruccion desde el diccionario de instrucciones
    // obtengo los parametros
    // traduce las direcciones de memoria en caso de ser necesario (logica -> fisica)
    instruccion_ejecutable instruccion;
    char **partes_instruccion = string_split(diccionario_instrucciones, " ");
    char *clave_instrucciones = partes_instruccion[0];
    void *funcion_instruccion = dictionary_get(diccionario_instrucciones, );

    instruccion.funcion_instruccion = funcion_instruccion;

    if (!strcmp(clave_instrucciones, "NOOP"))
    {
        char **parametros;

        for (int32_t i = 1; i < string_array_size(partes_instruccion); i++)
        {
            string_array_push(partes_instruccion[i]);
        }

        instruccion.parametros = parametros;

        // TODO: traduccion de direcciones de memoria para WRITE y READ -> puede ser que instruccion a ejecutar tenga otro campo "direccion fisica"
        // podria haber un modulo MMU que tiene las funciones de traduccion -> lo llamo aca adentro de un if que evalua si hay READ o WRITE
    } else {
        instruccion.parametros = NULL;
    }

    string_array_destroy(partes_instruccion);

    return instruccion;
}

void execute(instruccion_ejecutable instruccion, u_int32_t *program_counter)
{
    // ejecuta la funcion obtenida con los parametros necesarios
    //TODO: como voy a solucionar el tema de GOTO? tiene que acutualizar el valor del program_counter al que diga la instruccion
    //deberia modificar los params de la funcion de la instruccion?
    program_counter ++;
    
    instruccion.funcion_instruccion(instruccion.parametros);
}

void check_desalojo()
{
}

void check_interrupt()
{
}
