#define VMEM_IMPLEMENTATION
#define VLOG_LOG
#include "../include/vmem.h"

int main() {
  struct VMEM_Allocator *allocator = VMEM_AllocatorCreate(0);
  float *matrix = VMEM_Allocate(16 * sizeof(float), allocator);
  float *vector = VMEM_Allocate(3*sizeof(float), allocator);

  char *big_buffer1 = VMEM_Allocate(3 * 1024 * 1024, allocator);
  char *big_buffer2 = VMEM_Allocate(3 * 1024 * 1024, allocator);
  
  
  VMEM_AllocatorDebug(stdout, allocator);
  VMEM_AllocatorDestroy(allocator);
  
  return 0;
};  
