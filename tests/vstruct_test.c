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

  printf("Testing string concatenation: ");

  VST_String name, path1, path2, path3, full, expected, empty, left;
  empty = VST_StringCreate();
  expected = VST_StringFromStr("home/user/models/model.obj/");
  name = VST_StringFromStr("   model.obj/    ");
  VST_StringTrimWhitespace(name);  
  path1 = VST_StringFromStr("home");
  path2 = VST_StringFromStr("user");
  path3 = VST_StringFromStr("models");  
  full = VST_StringsConcatPath(path1, path2, path3, name);

  assert(VST_StringEqual(full, expected));
  assert(!VST_StringContainsStr(empty, "models"));  
  assert(VST_StringContainsStr(full, "models"));

  printf("\tOK\n");

  struct VST_StringView expected_parts[] = {
    VST_StringViewFromStr("home"),
    VST_StringViewFromStr("user"),
    VST_StringViewFromStr("models"),
    VST_StringViewFromStr("model.obj"),
    VST_StringViewFromStr("")
  };

  struct VST_StringView delimiter = VST_StringViewFromStr("/");
  
  printf("Testing searching for substrings: ");  
  usize expected_found_count = 4;
  assert(VST_StringFindAllCount(full, delimiter) ==
         expected_found_count);
  
  struct VST_StringView found_views[expected_found_count];
  VST_StringFindAll(full, delimiter, found_views);

  usize expected_part_positions[] = {
    4, 9, 16, 26
  };

  for (usize i = 0; i < expected_found_count; i++) {
    assert(found_views[i].offset == expected_part_positions[i]);
  }
  printf("\tOK\n");

  printf("Testing string splitting: ");
  usize expected_split_count = 5;
  assert(VST_StringSplitByCount(full, delimiter) == expected_split_count);

  struct VST_StringView split_views[expected_split_count];
  VST_StringSplitBy(full, delimiter, split_views);
  
  for (usize i = 0; i < expected_split_count; i++) {
    assert(VST_StringViewEqual(split_views[i], expected_parts[i]));    
  }

  printf("\tOK\n");
  
  VST_StringsDestroy(name, path1, path2, path3, full, expected, empty);  

  
  return 0;
}  
