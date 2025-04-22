#ifndef UTILS_STRING_H
#define UTILS_STRING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/string.h>

/**
 * @brief Convierte un array de strings en un solo string.
 *
 * @param string_array
 * @return `char*`
 *
 * @note El string resultante deberá ser liberado.
 */
char *array_to_string(char **string_array);

/**
 * @brief Retorna un nuevo array de strings sin el primer elemento.
 *
 * @param string_array
 * @return `char**`
 *
 * @note Duplica todos los elementos del array original,
 * no hay referencias compartidas.
 */
char **remove_first_element(char **string_array);

/**
 * @brief Elimina el salto de línea de un string.
 *
 * @param string
 * @return `char*`
 *
 * @note El string resultante deberá ser liberado.
 */
char *remove_new_line(char *string);

/**
 * @brief Retorna la longitud de un string incluyendo el terminador nulo.
 *
 * @param string
 * @return `u_int32_t`
 */
u_int32_t string_full_length(char *string);

/**
 * @brief Compara dos strings.
 *
 * @param string1
 * @param string2
 * @return `u_int8_t`
 *
 * @note Retorna 1 si son iguales, 0 en caso contrario.
 */
u_int8_t string_is_equal(char *string1, char *string2);

#endif // UTILS_STRING_H
