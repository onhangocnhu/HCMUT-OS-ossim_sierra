#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

int empty(struct queue_t *q)
{
    if (q == NULL)
        return 1;
    return (q->size == 0);
}

void enqueue(struct queue_t *q, struct pcb_t *proc)
{
    /* TODO: put a new process to queue [q] */
    if (q == NULL)
    {
        printf("The queue is null!\n");
        return;
    }
    if (proc == NULL)
    {
        printf("Process is null!\n");
        return;
    }

    // Check if the queue is full

    if (q->size >= MAX_QUEUE_SIZE)
    {
        printf("The queue is full! Cannot enqueue process.\n");
        return;
    }
    // Add the process to the queue
    // Check to avoid duplicating
    for (int i = 0; i < q->size; i++)
    {
        if (q->proc[i] == proc)
        {
            return;
        }
    }
    q->proc[q->size++] = proc;
}

struct pcb_t *dequeue(struct queue_t *q)
{
    /* TODO: return a pcb whose prioprity is the highest
     * in the queue [q] and remember to remove it from q
     * */
    // Check if the queue is null or empty
    if (empty(q) == 1)
    {
        printf("The queue is null or empty!\n");
        return NULL;
    }
    
    struct pcb_t *pop_proc = q->proc[0];

    // Remove pop_proc from queue
    for (int i = 0; i < q->size - 1; i++)
    {
        q->proc[i] = q->proc[i + 1];
    }
    q->proc[q->size - 1] = NULL;
    q->size--;
    return pop_proc;
}

void clear_queue(struct queue_t *q)
{
    while (!empty(q))
    {
        dequeue(q);
    }
    q->size = 0;
}
