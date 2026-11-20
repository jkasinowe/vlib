#ifndef VMEM_H
#define VMEM_H
#include "vmath.h"

typedef struct VMEM_Allocator *VMEM_Allocator;


#ifndef VMEM_ALLOCATOR_DEFAULT_BLOCK_SIZE
#define VMEM_ALLOCATOR_DEFAULT_BLOCK_SIZE (4 * 1024 * 1024)
#endif

#define VMEM_ALLOCATOR_FLAG_POOL (1 << 0)




struct VMEM_Allocator *VMEM_AllocatorCreate(usize flags);
struct VMEM_Allocator *VMEM_AllocatorCreateEx(usize block_size, usize flags);
void VMEM_AllocatorDestroy(VMEM_Allocator allocator);
void *VMEM_Allocate(usize size, VMEM_Allocator allocator);
void *VMEM_Reallocate(void* buffer, usize size);
void VMEM_AllocatorDebug(FILE* file, VMEM_Allocator allocator);

b8 VMEM_Copy(void* to, usize size, void* from, usize amount);

const char* VMEM_ErrorGet();


//#define VMEM_IMPLEMENTATION
#ifdef VMEM_IMPLEMENTATION
#include "vlog.h"
#include <string.h>

enum iVMEM_Error {
  IVMEM_ERROR_NO_ERROR,
  IVMEM_ERROR_DESTINATION_NULL,
  IVMEM_ERROR_SOURCE_NULL,
  IVMEM_ERROR_DESTINATION_SMALL,
};

enum iVMEM_Error iVMEM_ErrorLast = IVMEM_ERROR_NO_ERROR;

struct VMEM_AllocatorBlock {
  void *data;
  usize pointer;
};  

struct VMEM_Allocator {
  struct VMEM_AllocatorBlock* blocks;
  usize block_size;
  usize block_count;
};

#define iVMEM_ALIGNMENT 16ULL

struct VMEM_AllocatorBlock *
iVMEM_AllocatorBlockLastGet(struct VMEM_Allocator *allocator) {
  return &allocator->blocks[allocator->block_count-1];
}

void iVMEM_AllocatorBlockCreate(struct VMEM_Allocator *allocator) {
  VLOG_Wait("Creating a new allocator block");
  allocator->block_count++;
  allocator->blocks = (struct VMEM_AllocatorBlock *)VMEM_Reallocate(
      allocator->blocks,
      sizeof(struct VMEM_AllocatorBlock) * allocator->block_count);
  
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
  allocator->block_size = block_size;
  iVMEM_AllocatorBlockCreate(allocator); // ensure there is at minimum 1 block
  
  return allocator;
}

void *iVMEM_AlignAddress(void *ptr) {
  return (char*)((u64)((char*)ptr + (iVMEM_ALIGNMENT - 1)) & ~(iVMEM_ALIGNMENT - 1));
};  

void *iVMEM_AllocatorAllocate(struct VMEM_Allocator *allocator, usize size) {
  struct VMEM_AllocatorBlock *block_last =
      iVMEM_AllocatorBlockLastGet(allocator);

  char *block_last_bytes = (char *)block_last->data;

  void* element_start_address_no_padding = &block_last_bytes[block_last->pointer];
  
  void* element_start_address_with_padding = iVMEM_AlignAddress(element_start_address_no_padding);

  u64 padding = (char*)element_start_address_with_padding - (char*)element_start_address_no_padding;


  if (block_last->pointer + padding + size >= allocator->block_size) {    
    VLOG_Info("%zu+%zu+%zu=%zu, bigger than block size", block_last->pointer,
              size, padding, block_last->pointer + size + padding);    
    iVMEM_AllocatorBlockCreate(allocator);
    block_last = iVMEM_AllocatorBlockLastGet(allocator);
  }

  VLOG_Wait("Getting new allocation address");

  element_start_address_no_padding = &(block_last_bytes)[block_last->pointer];
  element_start_address_with_padding = iVMEM_AlignAddress(element_start_address_no_padding);

  VLOG_Info("%p", element_start_address_with_padding);
  VLOG_Info("%p", element_start_address_no_padding);
  padding = (char*)element_start_address_with_padding - (char*)element_start_address_no_padding;  
  
  block_last->pointer += padding+size;
  VLOG_Ok("Allocation address with 0x%x padding = %p", (u32)padding, element_start_address_with_padding);
  return element_start_address_with_padding;
}

void *VMEM_Allocate(usize size, struct VMEM_Allocator *allocator) {
  if (allocator) {
    return iVMEM_AllocatorAllocate(allocator, size);
  } else {
    return calloc(1, size);
  }    
}

void *VMEM_Reallocate(void *buffer, usize size) {
  if (!buffer) {
    return VMEM_Allocate(size, NULL);
  } else {
    return realloc(buffer, size);
  }  
}

void VMEM_AllocatorDebug(FILE* file, struct VMEM_Allocator *allocator) {
  fprintf(file, "Allocator at %p:\n", allocator);
  for (usize i = 0; i < allocator->block_count; i++) {
    void *block_start = allocator->blocks[i].data;
    usize pointer = allocator->blocks[i].pointer;
    void *block_end = (char*)block_start +pointer;
    fprintf(file, "block:\n");
    fprintf(file, "\taddress\t->%p\n", block_start);
    fprintf(file, "\tpointer\t->%p\t%zu\n", block_end, pointer);
  }    
}


void VMEM_AllocatorDestroy(struct VMEM_Allocator *allocator) {
  for (usize i = 0; i < allocator->block_count; i++) {
    free(allocator->blocks[i].data);
  }
  free(allocator->blocks);
  free(allocator);
}

b8 VMEM_Copy(void *to, usize size, void *from, usize amount) {
  if (!to) {
    iVMEM_ErrorLast = IVMEM_ERROR_DESTINATION_NULL;
    return 1;
  }    
  if (!from) {
    iVMEM_ErrorLast = IVMEM_ERROR_SOURCE_NULL;
    return 1;
  }    

  if (size < amount) {
    iVMEM_ErrorLast =  IVMEM_ERROR_DESTINATION_SMALL;
    return 1;
  }    

  memcpy(to, from, amount);
  
  return 0;
}


const char* VMEM_ErrorGet() {
  switch (iVMEM_ErrorLast) {
  case IVMEM_ERROR_NO_ERROR:
    return "";
  case IVMEM_ERROR_DESTINATION_NULL:
    return "Destination is NULL";
  case IVMEM_ERROR_DESTINATION_SMALL:
    return "Destination is too small";
  case IVMEM_ERROR_SOURCE_NULL:
    return "Source is NULL";
  }
  return "Unimplemented error";
}  

#endif // VMEM_IMPLEMENTATION

#endif // VMEM_H
