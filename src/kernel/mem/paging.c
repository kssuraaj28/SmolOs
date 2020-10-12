#include <stdint.h>
#include <mem/paging.h>
#include <debug/panic.h>

extern pd_entry_t __init_page_dir[];
extern uint8_t __kernel_pma[];
extern uint8_t __kernel_vma[];

page_dir_t _page_dir;
bool _is_identity_map = true; 

page_table_t init_pt; 

void flush_tlb()
{
  asm volatile (
      "mov %%cr3,%%eax\n"
      "mov %%eax,%%cr3\n"
      :::"eax");
}

void invlpg(void* addr)
{
  asm volatile (
      "invlpg %[addr]\n"
      ::[addr]"m"(addr));
}

void paging_init()
{
  if(!_is_identity_map) PANIC("No identity map");
  uint32_t phy = (uint32_t)__kernel_pma;
  uint32_t virt = (uint32_t)__kernel_vma;

  //Updating page table
  for(uint32_t offset=0;offset<PT_ENTRY_COUNT;offset++)
    init_pt.pages[offset] = ((offset*PAGE_SIZE) & PTE_FRAME)|PTE_WRITABLE|PTE_PRESENT;

  //Updating page dir
  uint32_t pd_index = EXTRACT_PD_INDEX(virt);
  uint32_t pt_phy = (uint32_t)&init_pt - (virt - phy);

  _page_dir.tables[pd_index] = (pt_phy&PDE_FRAME) | PDE_PRESENT | PDE_WRITABLE;
  _page_dir.table_virt[pd_index] = &init_pt;

  _page_dir.tables[0] = 0;
  _is_identity_map = 0;

  //Updating pdbr
  uint32_t pd_phy = (uint32_t)&_page_dir - (virt - phy);

  asm volatile ("mov %[pdbr],%%cr3\n"::[pdbr]"r"(pd_phy));
  //free the old block?
}
