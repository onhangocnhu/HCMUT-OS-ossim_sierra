/*
 * Copyright (C) 2025 pdnguyen of HCMC University of Technology VNU-HCM
 */

/* Sierra release
 * Source Code License Grant: The authors hereby grant to Licensee
 * personal permission to use and modify the Licensed Source Code
 * for the sole purpose of studying while attending the course CO2018.
 */

#include "common.h"
#include "syscall.h"
#include "stdio.h"
#include "libmem.h"
#include "queue.h"

int __sys_killall(struct pcb_t *caller, struct sc_regs *regs)
{
    char proc_name[100];
    uint32_t data;
    uint32_t memrg = regs->a1;

    int i = 0;
    data = 0;
    while (data != -1) 
    {
        libread(caller, memrg, i, &data);
        proc_name[i] = data;
        if (data == -1)
            proc_name[i] = '\0';
        i++;
    }
    printf("The procname retrieved from memregionid %d is \"%s\"\n", memrg, proc_name);

    // MLQ
    for (int prio = 0; prio < MAX_PRIO; prio++)
    {
        struct queue_t *queue = &caller->mlq_ready_queue[prio];

        if (empty(queue)) continue;

        struct queue_t tmp_queue = {.size = 0};

        while (!empty(queue))
        {
            struct pcb_t *proc = dequeue(queue);
            char *pname = strrchr(proc->path, '/');
            pname = (pname != NULL) ? pname + 1 : proc->path;

            if (strcmp(pname, proc_name) != 0)
            {
                enqueue(&tmp_queue, proc);
            }
            else
            {
                free(proc);
            }
        }
        *queue = tmp_queue;
    }
    // Running_list
    struct queue_t *runlist = caller->running_list;
    if (runlist != NULL && !empty(runlist))
    {
        struct queue_t tmp_runlist = {.size = 0};

        while (!empty(runlist))
        {
            struct pcb_t *proc = dequeue(runlist);
            char *pname = strrchr(proc->path, '/');
            pname = (pname != NULL) ? pname + 1 : proc->path;

            if (strcmp(pname, proc_name) != 0)
            {
                enqueue(&tmp_runlist, proc);
            }
            else
            {
                free(proc);
            }
        }

        *runlist = tmp_runlist;
    }

    return 0;
}
