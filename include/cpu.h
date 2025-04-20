#ifndef CPU_H
#define CPU_H

#include "common.h"

/* Execute an instruction of a process. Return 0
 * if the instruction is executed successfully.
 * Otherwise, return 1. */
int run(struct pcb_t *proc);
int alloc(struct pcb_t *proc, uint32_t size, uint32_t reg_index);
int free_data(struct pcb_t *proc, uint32_t reg_index);
int read(struct pcb_t *proc, uint32_t source, uint32_t offset, uint32_t destination);
int write(struct pcb_t *proc, BYTE data, uint32_t destination, uint32_t offset);
int run(struct pcb_t *proc);

#endif
