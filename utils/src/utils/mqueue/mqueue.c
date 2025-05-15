#include "mqueue.h"

t_mutex_queue *mqueue_create()
{
    t_mutex_queue *mqueue = malloc(sizeof(t_mutex_queue));
    mqueue->queue = queue_create();
    pthread_mutex_init(&(mqueue->mutex), NULL);
    return mqueue;
}

void *mqueue_pop(t_mutex_queue *mqueue)
{
    pthread_mutex_lock(&(mqueue->mutex));
    void *contenido = queue_pop(mqueue->queue);
    pthread_mutex_unlock(&(mqueue->mutex));
    return contenido;
}

// Ver si se usa
void *mqueue_peek(t_mutex_queue *mqueue)
{
    pthread_mutex_lock(&(mqueue->mutex));
    void *contenido = queue_peek(mqueue->queue);
    pthread_mutex_unlock(&(mqueue->mutex));
    return contenido;
}

void mqueue_push(t_mutex_queue *mqueue, void *item)
{
    pthread_mutex_lock(&(mqueue->mutex));
    queue_push(mqueue->queue, item);
    pthread_mutex_unlock(&(mqueue->mutex));
}

void mqueue_destroy(t_mutex_queue *mqueue)
{
    if (mqueue == NULL)
        return;

    pthread_mutex_lock(&(mqueue->mutex));
    queue_destroy(mqueue->queue);
    pthread_mutex_unlock(&(mqueue->mutex)); // ver si hace falte el unlock
    pthread_mutex_destroy(&(mqueue->mutex));
    free(mqueue);
    mqueue = NULL;
}
