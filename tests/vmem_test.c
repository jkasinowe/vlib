#define VMEM_IMPLEMENTATION
#define VLOG_LOG
#include "../include/vmem.h"

int main() {
  struct VMEM_Allocator *allocator = VMEM_AllocatorCreate(0);
  VMEM_Allocate(1, allocator);
  VMEM_Allocate(2, allocator);


  
  VMEM_AllocatorDebug(stdout, allocator);
  VMEM_AllocatorDestroy(allocator);
  
  return 0;
};  
