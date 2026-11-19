#define VLOG_LOG
#define VSTRUCT_IMPLEMENTATION
#define VMEM_IMPLEMENTATION
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

  printf("Testing strings: \n");


  VST_String name, path1, path2, path3, full, expected;
  expected = VST_StringFromStr("home/user/models/model.obj");
  name = VST_StringFromStr("model.obj");
  path1 = VST_StringFromStr("home");
  path2 = VST_StringFromStr("user");
  path3 = VST_StringFromStr("models");  
  full = VST_StringsConcatPath(path1, path2, path3, name);

  puts(VST_StringPtrGet(full));
  assert(VST_StringEqual(full, expected));  

  VST_StringsDestroy(name, path1, path2, path3, full, expected);  
  printf("\tOK\n");
  
  return 0;
}  
