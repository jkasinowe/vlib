#define VLIB_IMPLEMENTATION
#include "../include/vmem.h"

int main() {
  printf("Testing allocator.. ");
  fflush(stdout);
  struct VMEM_Allocator *allocator = VMEM_AllocatorCreate(0);
  char* byte = VMEM_Allocate(1, allocator);

  char *word = VMEM_Allocate(2, allocator);

  assert(word - byte == 16);
  
  VMEM_AllocatorDestroy(allocator);
  printf("\tOK\n");
  return 0;
};  
