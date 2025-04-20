#ifndef OSCFG_H
#define OSCFG_H

#define MLQ_SCHED 1 // chung, nào cũng phải có
#define MAX_PRIO 140

#define MM_PAGING      // chung, nào cũng phải có
#define MM_FIXED_MEMSZ // ./os sched, ./os sched_0, ./os sched_1, ./os os_1_singleCPU_mlq
#define VMDBG 1        // còn lại
#define MMDBG 1        // còn lại
#define IODUMP 1       // print DUMP
#define PAGETBL_DUMP 1 // Print page table
#define TIME_SLOT 1    // Print time slot
#endif
