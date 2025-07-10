#ifndef DUMP_H
#define DUMP_H

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "config/config.h"
#include "logger/logger.h"

void generar_dump(u_int32_t pid, t_list *paginas);

#endif // DUMP_H
