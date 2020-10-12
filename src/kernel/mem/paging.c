#include <stdint.h>
#include <mem/paging.h>
#include <debug/panic.h>

extern uint8_t __kernel_pma[];
extern uint8_t __kernel_vma[];

extern pd_entry_t _init_page_dir[];

page_dir_t _page_dir;

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

void map_page(page_dir_t* pd, void* virt, uint32_t page_frame, enum PAGE_PDE_FLAGS pd_flags, enum PAGE_PTE_FLAGS pt_flags)
{
  uint32_t virtual_address = (uint32_t)virt;

  if (virtual_address % PAGE_SIZE) PANIC("Unaligned virtual address");
  if (page_frame  & ~PTE_FRAME) PANIC("Unaligned physical address");

	uint32_t pd_index = EXTRACT_PD_INDEX(virtual_address);
  uint32_t pt_index = EXTRACT_PT_INDEX(virtual_address);

	if (!(pd -> tables[pd_index] & PDE_PRESENT))
	{
    PANIC("Kernel mem allocator not present yet");
	}
  
  pd -> tables[pd_index] |= pd_flags;

  page_table_t* page_table = pd -> table_virt[pd_index];
	page_table -> pages[pt_index] = (page_frame & PTE_FRAME) | pt_flags;
	
	invlpg(virt);
}

uint32_t free_page(page_dir_t* pd, void* virt)
{
  uint32_t va = (uint32_t)virt;

	uint32_t pd_index = EXTRACT_PD_INDEX(va);
  uint32_t pt_index = EXTRACT_PT_INDEX(va);

  if (!pd -> tables[pd_index] & PDE_PRESENT) PANIC ("No entry present");

  page_table_t* pt = pd -> table_virt[pd_index];

  if (!pt -> pages[pt_index] & PTE_PRESENT) PANIC ("No entry present");

  uint32_t block = pt -> pages[pt_index] & PTE_FRAME;
  pt -> pages[pt_index] ^= PTE_PRESENT;
  return block;
}
