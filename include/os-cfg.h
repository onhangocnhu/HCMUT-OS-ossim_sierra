#ifndef OSCFG_H
#define OSCFG_H

#define MLQ_SCHED 1
#define MAX_PRIO 140

#define MM_PAGING
#define MM_FIXED_MEMSZ // ./os sched, ./os sched_0, ./os sched_1, ./os os_1_singleCPU_mlq
#define VMDBG 1
#define MMDBG 1
#define IODUMP 1       // Print I/O Dump
#define PAGETBL_DUMP 1 // Print Page Table Dump
#define TIME_SLOT 1    // Print Time Slot

#endif