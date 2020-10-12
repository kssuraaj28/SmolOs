/* A page frame allocator (physical memory manager)*/

#include <stdint.h> 
#include <stdbool.h>

#include <multiboot.h>
#include <common.h>
#include <mem/paging.h>
#include <debug/qemu_print.h>
#include <debug/panic.h>

#define BLOCK_SIZE 4096
#define UPPER_MEM 0x100000
#define BLOCKS_PER_BUCKET (8 * sizeof(pfa_bucket_t))
#define SIZE_PER_BUCKET (BLOCK_SIZE * BLOCKS_PER_BUCKET)

#define BLOCK_NO(i) ((i-UPPER_MEM)/BLOCK_SIZE)
#define SET_BIT(i) pfa_bitmap[i / BLOCKS_PER_BUCKET] = pfa_bitmap[i / BLOCKS_PER_BUCKET] | (1 << (i % BLOCKS_PER_BUCKET))
#define CLEAR_BIT(i) pfa_bitmap[i / BLOCKS_PER_BUCKET] = pfa_bitmap[i / BLOCKS_PER_BUCKET] & (~(1 << (i % BLOCKS_PER_BUCKET)))
#define IS_SET(i) ((pfa_bitmap[i / BLOCKS_PER_BUCKET] >> (i % BLOCKS_PER_BUCKET)) & 0x1)

typedef uint8_t pfa_bucket_t;

extern uint8_t __kernel_vma[];
extern uint8_t __kernel_bss_end[];

uint32_t pfa_bitmap_size;
uint32_t _kernel_size;
pfa_bucket_t* pfa_bitmap = __kernel_bss_end;


void pfa_init(multiboot_info_t* mbd)
{ 
  if (!_is_identity_map) PANIC("No identity map");

  if (!mbd -> flags & MULTIBOOT_INFO_MEMORY) PANIC("No mem info");

  qemu_puts("\n\rLower memory: "); qemu_hex(mbd -> mem_lower * 1024);
  qemu_puts("\n\rHigher memory: "); qemu_hex(mbd -> mem_upper * 1024);

  uint32_t umem_size = mbd -> mem_upper * 1024;

  pfa_bitmap_size = umem_size / (SIZE_PER_BUCKET);

  _kernel_size =  (uint32_t)__kernel_bss_end - (uint32_t)__kernel_vma;
  _kernel_size += pfa_bitmap_size;


  qemu_puts("\n\rTotal Kernel size: "); qemu_hex(_kernel_size);
  qemu_puts("\n\rpfa_bitmap size: "); qemu_hex(pfa_bitmap_size);

  _kernel_size = (_kernel_size%SIZE_PER_BUCKET) ? ((_kernel_size/SIZE_PER_BUCKET)+1)*SIZE_PER_BUCKET : _kernel_size;
  
  memset(pfa_bitmap,0,pfa_bitmap_size);
  memset(pfa_bitmap,~0,_kernel_size/SIZE_PER_BUCKET);
}

uint32_t first_free_block()
{
  for (uint32_t i=0; i<pfa_bitmap_size; i++)
  {
    if (pfa_bitmap[i] == (pfa_bucket_t)-1) continue;

    uint32_t block_number = i * BLOCKS_PER_BUCKET;
    pfa_bucket_t bucket = pfa_bitmap[i];
    while(bucket & 1)
    {
      block_number++;
      bucket >>= 1;
    }
    return block_number;
  }
  return (uint32_t)-1;
}

uint32_t alloc_page_frame()
{
  uint32_t block = first_free_block();
  if (block == (uint32_t)-1) PANIC("No more blocks");

  SET_BIT(block);
	return (block * BLOCK_SIZE) + UPPER_MEM;
}

void free_page_frame(uint32_t address)
{
	if(address % BLOCK_SIZE) PANIC("Unaligned address");
  if(address < UPPER_MEM) PANIC("Low address");
  CLEAR_BIT(BLOCK_NO(address));
}
