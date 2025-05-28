#ifndef KERNEL_SYSCALLS_H
#define KERNEL_SYSCALLS_H

#include <stdlib.h>
#include <string.h>
#include <commons/string.h>
#include <utils/string/string.h>

#include "logger/logger.h"
#include "recursos/io.h"
#include "recursos/memoria.h"
#include "planificador/plani_largo_plazo.h"

void manejar_syscall(t_pcb *proceso, char *syscall);

#endif // KERNEL_SYSCALLS_H
