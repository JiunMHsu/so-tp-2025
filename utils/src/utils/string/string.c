#include "string.h"

char *array_to_string(char **string_array)
{
    char *string = string_new();
    for (int i = 0; string_array[i] != NULL; i++)
    {
        string_append(&string, string_array[i]);
        string_append(&string, " ");
    }
    string_trim_right(&string);

    return string;
}

char **remove_first_element(char **string_array)
{
    char **nuevo_array = string_array_new();

    for (int i = 1; string_array[i] != NULL; i++)
        string_array_push(&nuevo_array, strdup(string_array[i]));

    return nuevo_array;
}

char *remove_new_line(char *string)
{
    return string_replace(string, "\n", "");
}

u_int32_t string_full_length(char *string)
{
    return strlen(string) + 1;
}

u_int8_t string_is_equal(char *string1, char *string2)
{
    return strcmp(string1, string2) == 0;
}
