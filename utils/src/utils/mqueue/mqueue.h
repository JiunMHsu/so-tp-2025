#ifndef UTILS_MQUEUE_H
#define UTILS_MQUEUE_H

#include <stdlib.h>
#include <pthread.h>
#include <commons/collections/queue.h>

typedef struct
{
    t_queue *queue;
    pthread_mutex_t mutex;
} t_mutex_queue;

t_mutex_queue *crear_mutex_queue(void);
void *pop_mutex_queue(t_mutex_queue *mqueue);
void *peek_mutex_queue(t_mutex_queue *mqueue);
void push_mutex_queue(t_mutex_queue *mqueue, void *item);
void destruir_mutex_queue(t_mutex_queue *mqueue);

#endif // UTILS_MQUEUE_H
