# Assignment - Simple Operating System

## **Information**
-   **Course**: Operating Systems (CO2017)
-   **Organization**: University of Technology, Vietnam National University Ho Chi Minh City
-   **Project**: Simple Operating System
-   **Class & Group**: L13 - Group 06
-   **Semester**: 242 (Year of 2024 - 2025)
-   **Objective**: The simulation of major components in a simple operating system includes scheduler, synchronization, related operations of physical memory and virtual memory

## **📂 Project Structure**
```
ossim_sierra/
 ├── include/                               # Header files
 │   ├── bitops.h
 │   ├── common.h
 │   ├── cpu.h
 │   ├── libmem.h
 │   ├── loader.h            # Obsoleted
 │   ├── mem.h
 │   ├── mm.h
 │   ├── os-cfg.h
 │   ├── os-mm.h
 │   ├── queue.h
 │   ├── sched.h
 │   ├── syscall.h
 │   ├── timer.h 
 ├── input/                                 # List of processes and test cases
 │   ├── proc/
 │   │   ├── m0s
 │   │   ├── m1s
 │   │   ├── p0s
 │   │   ├── p1s
 │   │   ├── p2s
 │   │   ├── p3s
 │   │   ├── s0
 │   │   ├── s1
 │   │   ├── s2
 │   │   ├── s3
 │   │   ├── s4
 │   │   ├── sc1
 │   │   ├── sc2
 │   │   ├── sc3  
 │   ├── os_0_mlq_paging
 │   ├── os_1_mlq_paging
 │   ├── os_1_mlq_paging_small_1K
 │   ├── os_1_mlq_paging_small_4K
 │   ├── os_1_singleCPU_mlq
 │   ├── os_1_singleCPU_mlq_paging
 │   ├── os_sc
 │   ├── os_syscall
 │   ├── sched
 │   ├── sched_0
 │   ├── sched_1  
 ├── obj/                                   # Object files
 │   ├── cpu.o
 │   ├── libmem.o
 │   ├── libstd.o 
 │   ├── loader.o
 │   ├── mem.o
 │   ├── mm-memphy.o
 │   ├── mm-vm.o  
 │   ├── mm.o
 │   ├── os.o
 │   ├── queue.o
 │   ├── sched.o
 │   ├── sys_killall.o
 │   ├── sys_listsyscall.o
 │   ├── sys_mem.o 
 │   ├── sys_xxxhandler.o 
 │   ├── syscall.o
 │   ├── timer.o 
 ├── output/                                 # Results of test cases (no need to fully similar)
 │   ├── os_0_mlq_paging.output
 │   ├── os_1_mlq_paging.output
 │   ├── os_1_mlq_paging_small_1K.output
 │   ├── os_1_mlq_paging_small_4K.output
 │   ├── os_1_singleCPU_mlq.output
 │   ├── os_1_singleCPU_mlq_paging.output
 │   ├── os_sc.output
 │   ├── os_syscall.output
 │   ├── sched.output
 │   ├── sched_0.output
 │   ├── sched_1.output   
 ├── src/
 │   ├── cpu.c
 │   ├── libmem.c
 │   ├── libstd.c 
 │   ├── loader.c
 │   ├── mem.c               # Obsoleted     
 │   ├── mm-memphy.c
 │   ├── mm-vm.c  
 │   ├── mm.c
 │   ├── os.c
 │   ├── paging.c            # Obsoleted
 │   ├── queue.c
 │   ├── sched.c
 │   ├── sys_killall.c
 │   ├── sys_listsyscall.c
 │   ├── sys_mem.c 
 │   ├── sys_xxxhandler.c 
 │   ├── syscall.c
 │   ├── syscall.tbl
 │   ├── syscalltbl.lst
 │   ├── syscalltbl.sh
 │   ├── timer.c 
 ├── Makefile 
 ├── os 
 ├── README.md                              # Documentation
```

---

## Getting Started

### Prerequisites

Make sure your OS has the GCC installed by using the following command:

```sh
gcc --version
```

If the OS has not installed GCC yet, you should install it by online documents.


## Run the Program

Compiled the source codes by using Makefile:

```sh
make all
```

Run the simulation:

```sh
./os <configure_file>
```

Where **configure file** is the path to configure file for the environment on which you want to run and it should associated with the name of a description file placed in **input** directory.

**Example**

```sh
./os os_0_mlq_paging
```

## Clean the Program

To remove compiled files and clean up the project directory, run:

```sh
make clean
```

## Description of configure files in *input* directory

- **Line 1:**  
  `time_slice` `N` `M`
  where `N` = Number of CPUs, `M` = Number of processes to be run

- **Line 2:**  
  `RAM_SZ` `SWP_SZ_0` `SWP_SZ_1` `SWP_SZ_2` `SWP_SZ_3`

- **Line 3 to Line (3 + M - 1):**  
  Each line represents one process with the format:  
  `time_i` `path_i` `priority_i`  
  where `i = 0` to `M - 1`

Which **path_i** is the name of i-th process in **proc** directory.

**Example**

In `input/os_0_mlq_paging`:

```
6 2 4
1048576 16777216 0 0 0
0 p0s 0
2 p1s 15
4 p1s 0
6 p1s 0
```

## Description of processes

Each process in the `proc/` directory follows this format:

- **Line 1:**  
  `priority` `N` = number of instructions

- **Line 2 to Line (2 + N - 1):**  
  Each line contains one instruction:
```
instruction_0
instruction_1
...
instruction_{N-1}
```

**Example**

In `input/proc/m1s`:

```
1 6
alloc 300 0
alloc 100 1
free 0
alloc 100 2
free 2
free 1
```