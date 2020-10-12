#ifndef MEM_PAGING_H
#define MEM_PAGING_H 

#include <stdint.h>
#include <stdbool.h>
#include <mem/pfa.h>

#define PT_ENTRY_COUNT 1024
#define PD_ENTRY_COUNT 1024
#define PAGE_SIZE 4096

#define EXTRACT_PD_INDEX(x) (x>>22)
#define EXTRACT_PT_INDEX(x) ((x>>12)&0x3FF)


enum PAGE_PTE_FLAGS {
    PTE_PRESENT		=	1,
    PTE_WRITABLE		=	2,
    PTE_USER		=	4,
    PTE_WRITETHOUGH		=	8,
    PTE_NOT_CACHEABLE	=	0x10,
    PTE_ACCESSED		=	0x20,
    PTE_DIRTY		=	0x40,
    PTE_PAT			=	0x80,
    PTE_CPU_GLOBAL		=	0x100,
    PTE_LV4_GLOBAL		=	0x200,
    PTE_FRAME		=	0xFFFFF000 
};

enum PAGE_PDE_FLAGS {
	PDE_PRESENT		=	1,
	PDE_WRITABLE	=	2,
	PDE_USER		  =	4,
	PDE_PWT			  =	8,	
	PDE_PCD			  =	0x10,
	PDE_ACCESSED	=	0x20,
	PDE_DIRTY		  =	0x40,
	PDE_4MB			  =	0x80,
	PDE_CPU_GLOBAL=	0x100,
	PDE_LV4_GLOBAL=	0x200,	
  PDE_FRAME		  =	0xFFFFF000 
};

typedef uint32_t pt_entry_t;
typedef uint32_t pd_entry_t;

typedef struct __attribute__((aligned(4096))) __attribute__((packed)) page_table {
  pt_entry_t pages[PT_ENTRY_COUNT];
} page_table_t;

typedef struct __attribute__((aligned(4096))) __attribute__((packed)) page_dir {
  pd_entry_t tables[PD_ENTRY_COUNT];
  page_table_t* table_virt[PD_ENTRY_COUNT];
} page_dir_t;

extern page_dir_t _page_dir;
extern bool _is_identity_map;

#define MAP_K(x,y) (map_page(&_page_dir,x,y,PDE_PRESENT|PDE_WRITABLE,PTE_PRESENT|PTE_WRITABLE))
#define FREE_K(x) (free_page_frame(free_page(&_page_dir,x)))

void paging_init();
void flush_tlb();
void invlpg(void* addr);
void map_page(page_dir_t* pd, void* virt, uint32_t page_frame, enum PAGE_PDE_FLAGS pd_flags, enum PAGE_PTE_FLAGS pt_flags);
uint32_t free_page(page_dir_t* pd, void* virt);

#endif
