#include "queue.h"

void clear(Queue *queue)
{
    queue->head = queue->tail = 0;
}

void enqueue(Queue *queue, char *element)
{
    queue->buffer[(queue->tail)++] = element;
}

char* dequeue(Queue *queue)
{
    return queue->buffer[(queue->head)++];
}

void resetIfNeeded(Queue *queue)
{
    if (queue->tail <= queue->head)
    {
        queue->tail = queue->head = 0;
    }
}

int empty(Queue *queue)
{
    return queue->tail == queue->head;
}

int size(Queue *queue)
{
    return queue->tail - queue->head;
}
