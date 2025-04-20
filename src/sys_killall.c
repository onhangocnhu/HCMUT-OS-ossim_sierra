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

    // hardcode for demo only
    uint32_t memrg = regs->a1;

    /* TODO: Get name of the target proc */
    // proc_name = libread..
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

    /* TODO: Traverse proclist to terminate the proc
     *       stcmp to check the process match proc_name
     */

    // caller->running_list
    // caller->mlq_ready_queue

    /* TODO Maching and terminating
     *       all processes with given
     *        name in var proc_name
     */
    // Duyệt qua MLQ
    for (int prio = 0; prio < MAX_PRIO; prio++)
    {
        struct queue_t *queue = &caller->mlq_ready_queue[prio];

        int size = queue->size;
        for (int i = 0; i < size; i++)
        {
            struct pcb_t *proc = queue->proc[i];
            char *pname = strrchr(proc->path, '/');
            if (pname != NULL)
            {
                pname++;
            }
            else
            {
                pname = proc->path;
            }

            if (strcmp(pname, proc_name) == 0)
            {
                free(proc);
                for (int j = i; j < size - 1; j++)
                {
                    queue->proc[j] = queue->proc[j + 1];
                }
                queue->size--;
                size--; // cập nhật lại size
                i--;    // kiểm tra lại vị trí hiện tại (do bị dồn phần tử)
            }
        }
    }
    // Duyệt qua running_list
    if (caller->running_list != NULL)
    {
        int size = caller->running_list->size;
        for (int i = 0; i < size; i++)
        {
            struct pcb_t *proc = caller->running_list->proc[i];
            char *pname = strrchr(proc->path, '/');
            if (pname != NULL)
            {
                pname++;
            }
            else
            {
                pname = proc->path;
            }

            if (strcmp(pname, proc_name) == 0)
            {
                free(proc);
                for (int j = i; j < size - 1; j++)
                {
                    caller->running_list->proc[j] = caller->running_list->proc[j + 1];
                }
                caller->running_list->proc[size - 1] = NULL; // Đảm bảo phần tử cuối là NULL
                caller->running_list->size--;
                size--;
                i--; // Kiểm tra lại vị trí hiện tại
            }
        }
    }

    return 0;
}

/* Trả lời câu hỏi:
Question: What is the mechanism to pass a complex argument to a system call using the limited registers?

Ans:
Do có ít thanh ghi a0, a1..., đối số phức tạp nên không thể truyền trực tiếp (pass the parameters in registers)
mà phải truyền tham chiếu (Parameters stored in a block, or table, in memory, and address of block
passed as a parameter in a register)

Địa chỉ này sẽ được nạp vào 1 register (ví dụ như a1),
system call sẽ nhận địa chỉ và truy xuất dữ liệu bằng cách đọc từ vùng nhớ (thông qua libread())
*/