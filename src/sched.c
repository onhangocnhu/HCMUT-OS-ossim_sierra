#include "queue.h"
#include "sched.h"
#include <pthread.h>

#include <stdlib.h>
#include <stdio.h>
static struct queue_t ready_queue;
static struct queue_t run_queue;
static pthread_mutex_t lock;

static struct queue_t running_list;
#ifdef MLQ_SCHED
static struct queue_t mlq_ready_queue[MAX_PRIO];
static int slot[MAX_PRIO];
#endif

int queue_empty(void)
{
#ifdef MLQ_SCHED
    unsigned long prio;
    for (prio = 0; prio < MAX_PRIO; prio++)
        if (!empty(&mlq_ready_queue[prio]))
            return -1;
#endif
    return (empty(&ready_queue) && empty(&run_queue));
}

void init_scheduler(void)
{
#ifdef MLQ_SCHED
    int i;

    for (i = 0; i < MAX_PRIO; i++)
    {
        mlq_ready_queue[i].size = 0;
        slot[i] = MAX_PRIO - i;
    }
#endif
    ready_queue.size = 0;
    run_queue.size = 0;
    running_list.size = 0;
    pthread_mutex_init(&lock, NULL);
}

#ifdef MLQ_SCHED
/*
 *  Stateful design for routine calling
 *  based on the priority and our MLQ policy
 *  We implement stateful here using transition technique
 *  State representation   prio = 0 .. MAX_PRIO, curr_slot = 0..(MAX_PRIO - prio)
 */
struct pcb_t *get_mlq_proc(void)
{
    struct pcb_t *proc = NULL;
    /*TODO: get a process from PRIORITY [ready_queue].
     * Remember to use lock to protect the queue.
     */

    pthread_mutex_lock(&lock);
    int i = 0; // Ly(14/04/2025)_edit : Remove "static"
    int check = 0;
    for (;; i = (i + 1) % MAX_PRIO)
    {
        if (slot[i] == 0)
        {
            slot[i] = MAX_PRIO - i;
            check = 0;
            continue;
        }
        if (empty(&mlq_ready_queue[i]))
        {
            check++;
            if (check == MAX_PRIO)
                break;
            continue;
        }
        check = 0;
        slot[i]--;
        proc = dequeue(&mlq_ready_queue[i]);
        break;
    }

    pthread_mutex_unlock(&lock);
    return proc;
}

void put_mlq_proc(struct pcb_t *proc)
{
    pthread_mutex_lock(&lock);
    enqueue(&mlq_ready_queue[proc->prio], proc);
    pthread_mutex_unlock(&lock);
}

void add_mlq_proc(struct pcb_t *proc)
{
    pthread_mutex_lock(&lock);
    enqueue(&mlq_ready_queue[proc->prio], proc);
    // printf("Adding process PID: %d, PRIO: %d\n", proc->pid, proc->prio);
    pthread_mutex_unlock(&lock);
}

struct pcb_t *get_proc(void)
{
    return get_mlq_proc();
}

void put_proc(struct pcb_t *proc)
{
    proc->ready_queue = &ready_queue;
    proc->mlq_ready_queue = mlq_ready_queue;
    proc->running_list = &running_list;

    /* TODO: put running proc to running_list */
    pthread_mutex_lock(&lock);
    enqueue(&running_list, proc);
    pthread_mutex_unlock(&lock);
    put_mlq_proc(proc);
}

void add_proc(struct pcb_t *proc)
{
    proc->ready_queue = &ready_queue;
    proc->mlq_ready_queue = mlq_ready_queue;
    proc->running_list = &running_list;

    /* TODO: put running proc to running_list */
    pthread_mutex_lock(&lock);
    enqueue(&running_list, proc);
    pthread_mutex_unlock(&lock);
    add_mlq_proc(proc);
}
#else
struct pcb_t *get_proc(void)
{
    struct pcb_t *proc = NULL;
    /*TODO: get a process from [ready_queue].
     * Remember to use lock to protect the queue.
     * */
    pthread_mutex_lock(&lock);
    if (!empty(&ready_queue))
    {
        proc = dequeue(&ready_queue);
    }
    printf("Putting process PID: %d, PRIO: %d back to queue\n", proc->pid, proc->prio);
    pthread_mutex_unlock(&lock);
    return proc;
}

void put_proc(struct pcb_t *proc)
{
    proc->ready_queue = &ready_queue;
    proc->running_list = &running_list;

    /* TODO: put running proc to running_list */

    pthread_mutex_lock(&lock);
    enqueue(&running_list, proc);
    enqueue(&run_queue, proc);
    printf("Putting process PID: %d, PRIO: %d back to queue\n", proc->pid, proc->prio);
    pthread_mutex_unlock(&lock);
}

void add_proc(struct pcb_t *proc)
{
    proc->ready_queue = &ready_queue;
    proc->running_list = &running_list;

    /* TODO: put running proc to running_list */

    pthread_mutex_lock(&lock);
    enqueue(&running_list, proc);
    enqueue(&ready_queue, proc);
    pthread_mutex_unlock(&lock);
}
#endif
