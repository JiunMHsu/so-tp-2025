#include "sistema.h"
//TODO
// lista de pids con las instrucciones

t_dictionary *procesos;

// cuando se quiere crear un proceso

//Nico
// 1. leer el archivo de instrucciones
// 2. parsear las instrucciones a lista ->["NOOP", "WRITE 0 EJEMPLO_DE_ENUNCIADO", ...]
// 3. guardar en diccionario {clave: pid, valor: lista de instrucciones}


//Ale
// si cpu te pide una instruccion segun pid y pc
// - buscar en el diccionario la lista de instrucciones
