// #ifdef MM_PAGING
/*
 * PAGING based Memory Management
 * Virtual memory module mm/mm-vm.c
 */

#include "string.h"
#include "mm.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
static pthread_mutex_t mem_lock = PTHREAD_MUTEX_INITIALIZER;

/*get_vma_by_num - get vm area by numID
 *@mm: memory region
 *@vmaid: ID vm area to alloc memory region
 *
 */
struct vm_area_struct *get_vma_by_num(struct mm_struct *mm, int vmaid)
{
  pthread_mutex_lock(&mem_lock);
  struct vm_area_struct *pvma = mm->mmap;

  if (mm->mmap == NULL)
  {
    pthread_mutex_unlock(&mem_lock);
    return NULL;
  }

  int vmait = pvma->vm_id;
  while (vmait < vmaid)
  {
    if (pvma == NULL)
    {
      pthread_mutex_unlock(&mem_lock);
      return NULL;
    }

    pvma = pvma->vm_next;
    vmait = pvma->vm_id;
  }

  pthread_mutex_unlock(&mem_lock);
  return pvma;
}

int __mm_swap_page(struct pcb_t *caller, int vicfpn, int swpfpn)
{
  __swap_cp_page(caller->mram, vicfpn, caller->active_mswp, swpfpn);
  return 0;
}

/*get_vm_area_node - get vm area for a number of pages
 *@caller: caller
 *@vmaid: ID vm area to alloc memory region
 *@incpgnum: number of page
 *@vmastart: vma end
 *@vmaend: vma end
 *
 */
struct vm_rg_struct *get_vm_area_node_at_brk(struct pcb_t *caller, int vmaid, int size, int alignedsz)
{
  struct vm_rg_struct *newrg = malloc(sizeof(struct vm_rg_struct));

  if (!newrg)
  {
    return 0;
  }
  /* TODO retrive current vma to obtain newrg, current comment out due to compiler redundant warning*/
  // TODO: 11/04/2025
  struct vm_area_struct *cur_vma = get_vma_by_num(caller->mm, vmaid);

  if (!cur_vma)
  {
    free(newrg);
    return 0;
  }

  /* TODO: update the newrg boundary */
  // TODO: 11/04/2025
  newrg->rg_start = cur_vma->sbrk;
  // newrg->rg_end = newrg->rg_start + size;
  // TODO: 16/4/2025
  newrg->rg_end = newrg->rg_start + alignedsz;
  // printf("=== get_vm_area_node_at_brk ===\n");
  // printf("sbrk = %ld (0x%08lX), alignedsz = %d\n", cur_vma->sbrk, cur_vma->sbrk, alignedsz);

  return newrg;
}

/*validate_overlap_vm_area
 *@caller: caller
 *@vmaid: ID vm area to alloc memory region
 *@vmastart: vma end
 *@vmaend: vma end
 *
 */
int validate_overlap_vm_area(struct pcb_t *caller, int vmaid, int vmastart, int vmaend)
{
  pthread_mutex_lock(&mem_lock);

  /* TODO validate the planned memory area is not overlapped */
  // TODO: 11/04/2025
  struct vm_area_struct *vma = caller->mm->mmap;

  while (!vma)
  {
    if ((vma->vm_id != vmaid) && (vma->vm_start != vma->vm_end))
    {
      if ((int)((vmaend - 1 - vma->vm_start) * (vma->vm_end - 1 - vmastart)) >= 0)
      {
        pthread_mutex_unlock(&mem_lock);
        return -1;
      }
      if ((int)((vmastart - vma->vm_start) * (vma->vm_end - vmaend)) >= 0)
      {
        pthread_mutex_unlock(&mem_lock);
        return -1;
      }
    }
    vma = vma->vm_next;
  }

  pthread_mutex_unlock(&mem_lock);
  return 0;
}

/*inc_vma_limit - increase vm area limits to reserve space for new variable
 *@caller: caller
 *@vmaid: ID vm area to alloc memory region
 *@inc_sz: increment size
 *
 */
int inc_vma_limit(struct pcb_t *caller, int vmaid, int inc_sz)
{
  struct vm_rg_struct *newrg = malloc(sizeof(struct vm_rg_struct));
  int inc_amt = PAGING_PAGE_ALIGNSZ(inc_sz);
  int incnumpage = inc_amt / PAGING_PAGESZ;
  struct vm_rg_struct *area = get_vm_area_node_at_brk(caller, vmaid, inc_sz, inc_amt);

  struct vm_area_struct *cur_vma = get_vma_by_num(caller->mm, vmaid);
  int old_end = cur_vma->vm_end;

  /*Validate overlap of obtained region */
  if (validate_overlap_vm_area(caller, vmaid, area->rg_start, area->rg_end) < 0)
  {
    return -1; /*Overlap and failed allocation */
  }

  /* TODO: Obtain the new vm area based on vmaid */
  // TODO: 11/4/2025
  // cur_vma->vm_end += inc_sz;
  // cur_vma->sbrk += inc_sz;

  // TODO: 16/04/2025
  if (cur_vma != NULL && area != NULL)
  {
    cur_vma->sbrk = area->rg_end;
    cur_vma->vm_end = area->rg_end;
  }

  else
  {
    return -1;
  }

  if (vm_map_ram(caller, area->rg_start, area->rg_end, old_end, incnumpage, newrg) < 0)
  {
    // printf("[vm_map_ram] Mapping region: %08lX -> %08lX (size %ld)\n",
    //        area->rg_start, area->rg_end, area->rg_end - area->rg_start);

    return -1; /* Map the memory to MEMRAM */
  }

  return 0;
}

// #endif
