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

t_mutex_queue *mqueue_create(void);
void *mqueue_pop(t_mutex_queue *mqueue);
void *mqueue_peek(t_mutex_queue *mqueue);
void mqueue_push(t_mutex_queue *mqueue, void *item);
void mqueue_destroy(t_mutex_queue *mqueue);

#endif // UTILS_MQUEUE_H
