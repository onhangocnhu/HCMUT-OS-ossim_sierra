#ifndef QUEUE_H
#define QUEUE_H

#include "common.h"

#ifndef MLQ_SCHED
#define MLQ_SCHED
#endif

#ifdef MLQ_SCHED
// Bổ sung
int get_slot(uint32_t prio);                    // Trả về số slot còn lại
void update_slot(uint32_t prio, int used_time); // Cập nhật slot khi proc chạy
void reset_slot();                              // reset lại slot của tất cả hàng đợi mlq sau khi chạy xong 1 cycle

struct pcb_t *get_mlq_proc(void);
void put_mlq_proc(struct pcb_t *proc);
void add_mlq_proc(struct pcb_t *proc);
#endif

#define MAX_PRIO 140

int queue_empty(void);

void init_scheduler(void);
void finish_scheduler(void);

/* Get the next process from ready queue */
struct pcb_t *get_proc(void);

/* Put a process back to run queue */
void put_proc(struct pcb_t *proc);

/* Add a new process to ready queue */
void add_proc(struct pcb_t *proc);

#endif
