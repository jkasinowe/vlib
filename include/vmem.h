#ifndef VMEM_H
#define VMEM_H
#include "vmath.h"

struct VMEM_Allocator;

#ifndef VMEM_ALLOCATOR_DEFAULT_BLOCK_SIZE
#define VMEM_ALLOCATOR_DEFAULT_BLOCK_SIZE (4 * 1024 * 1024)
#endif

#define VMEM_ALLOCATOR_FLAG_POOL (1 << 0)




struct VMEM_Allocator *VMEM_AllocatorCreate(usize flags);
struct VMEM_Allocator *VMEM_AllocatorCreateEx(usize block_size, usize flags);
void VMEM_AllocatorDestroy(VMEM_Allocator *allocator);
void *VMEM_Allocate(usize size, struct VMEM_Allocator *allocator);
void *VMEM_Reallocate(void* buffer, usize size);


#endif // VMEM_H
#define VMEM_IMPLEMENTATION
#ifdef VMEM_IMPLEMENTATION
#include "vlog.h"


struct VMEM_AllocatorBlock {
  void *data;
  usize pointer;
};  

struct VMEM_Allocator {
  struct VMEM_AllocatorBlock* blocks;
  usize block_size;
  usize block_count;
};

struct VMEM_AllocatorBlock *
iVMEM_AllocatorBlockLastGet(struct VMEM_Allocator *allocator) {
  return &allocator->blocks[allocator->block_count-1];
}

void iVMEM_AllocatorBlockCreate(struct VMEM_Allocator *allocator) {
  VLOG_Wait("Creating a new allocator block");
  allocator->block_count++;
  if (allocator->block_count == 1) {
    allocator->blocks = (struct VMEM_AllocatorBlock *)VMEM_Allocate(
        sizeof(struct VMEM_AllocatorBlock), NULL);
  } else {
    VMEM_Reallocate(allocator->blocks, sizeof(struct VMEM_AllocatorBlock) *
                                           allocator->block_count);
  }
  struct VMEM_AllocatorBlock *block_last =
      iVMEM_AllocatorBlockLastGet(allocator);

  block_last->pointer = 0;
  block_last->data = VMEM_Allocate(allocator->block_size, NULL);
  VLOG_Ok("Allocator block created");
}


struct VMEM_Allocator *VMEM_AllocatorCreate(usize flags) {
  return VMEM_AllocatorCreateEx(VMEM_ALLOCATOR_DEFAULT_BLOCK_SIZE, flags);
}

struct VMEM_Allocator *VMEM_AllocatorCreateEx(usize block_size, usize flags) {
  struct VMEM_Allocator *allocator = (struct VMEM_Allocator *)VMEM_Allocate(
      sizeof(struct VMEM_Allocator), NULL);
  iVMEM_AllocatorBlockCreate(allocator); // ensure there is at minimum 1 block
  
  return allocator;
}


void *iVMEM_AllocatorAllocate(struct VMEM_Allocator *allocator, usize size) {
  struct VMEM_AllocatorBlock *block_last =
      iVMEM_AllocatorBlockLastGet(allocator);

  if (block_last->pointer + size >= allocator->block_size) {
    iVMEM_AllocatorBlockCreate(allocator);
    block_last = iVMEM_AllocatorBlockLastGet(allocator);
  }

  VLOG_Wait("Getting new allocation address");
  void* element_start_address = &((char*)block_last->data)[block_last->pointer];
  block_last->pointer += size;
  VLOG_Ok("Allocation address = %p", element_start_address);
  return element_start_address;
}

void *VMEM_Allocate(usize size, struct VMEM_Allocator *allocator) {
  if (allocator) {
    return iVMEM_AllocatorAllocate(allocator, size);
  } else {
    return calloc(1, size);
  }    
}

void *VMEM_Reallocate(void* buffer, usize size) {
  return realloc(buffer, size);
}  

#endif
