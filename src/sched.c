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
            return 0;
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

void finish_scheduler(void)
{
    clear_queue(&ready_queue);
    clear_queue(&run_queue);
    clear_queue(&running_list);

#ifdef MLQ_SCHED
    for (int i = 0; i < MAX_PRIO; i++)
    {
        clear_queue(&mlq_ready_queue[i]);
    }
#endif

    pthread_mutex_destroy(&lock);
}

#ifdef MLQ_SCHED
/*
 *  Stateful design for routine calling
 *  based on the priority and our MLQ policy
 *  We implement stateful here using transition technique
 *  State representation   prio = 0 .. MAX_PRIO, curr_slot = 0..(MAX_PRIO - prio)
 */
void update_slot(uint32_t prio, int used_time)
{
    // printf("update_slot[%d]\n ", used_time);
    // printf("Before:\nslot[%d]: %d\n ", prio, slot[prio]);
    slot[prio] = slot[prio] - used_time;
    // printf("After:\nslot[%d]: %d\n ", prio, slot[prio]);
}
void reset_slot()
{
    for (int i = 0; i < MAX_PRIO; i++)
    {
        slot[i] = MAX_PRIO - i;
    }
}
int get_slot(uint32_t prio)
{
    return slot[prio];
}

struct pcb_t *get_mlq_proc(void)
{
    struct pcb_t *proc = NULL;
    int curr_prio = 0;
    int count_empty = 0;

    pthread_mutex_lock(&lock);

    while (1)
    {
        // printf("curr_prio: %d\n", curr_prio);

        if (!empty(&mlq_ready_queue[curr_prio]))
        {
            if (slot[curr_prio] > 0)
            {
                // Found a process to run
                proc = dequeue(&mlq_ready_queue[curr_prio]);
                break;
            }
            else
            {
                // Queue has process but no slot left, move to next
                curr_prio = (curr_prio + 1) % MAX_PRIO;
                if (curr_prio == 0)
                {
                    count_empty = 0;
                    reset_slot();
                }
                continue;
            }
        }
        else
        {
            // Queue is empty
            count_empty++;
            if (count_empty == MAX_PRIO)
            {
                // All queues are empty
                break;
            }

            curr_prio = (curr_prio + 1) % MAX_PRIO;
            if (curr_prio == 0)
            {
                count_empty = 0;
                reset_slot();
            }
            continue;
        }
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
