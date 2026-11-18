#define VSTRUCT_IMPLEMENTATION
#include "../include/vstruct.h"
#include "../include/vmath.h"


int main() {
  printf("Testing array lists: ");
    
  u64 *ints = NULL;

  for (uint32_t i = 0; i < 10; i++) {
    VST_ListPush(ints, i);
    assert(ints[i] == i);
  }

  VST_ListFree(ints);
  printf("\tOK\n");
  
  return 0;
}  
