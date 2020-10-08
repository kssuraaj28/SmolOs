#ifndef MEM_PAGING_H
#define MEM_PAGING_H 

#include <stdint.h>

#define PT_ENTRY_COUNT 1024
#define PD_ENTRY_COUNT 1024
#define PAGE_SIZE 4096

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

typedef struct page_table {
  pt_entry_t pages[PT_ENTRY_COUNT];
} page_table_t;

typedef struct page_dir {
  pd_entry_t tables[PD_ENTRY_COUNT];
  page_table_t* table_virt[PD_ENTRY_COUNT];
} page_dir_t;

extern page_dir_t* _page_dir;

#endif
