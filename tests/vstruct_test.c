#define VLOG_LOG
#define VLIB_IMPLEMENTATION
#include "../include/vmath.h"
#include "../include/vstruct.h"

int main() {
  printf("Testing array lists: ");

  u64 *ints = NULL;

  for (uint32_t i = 0; i < 10; i++) {
    VST_ListPush(ints, i);
    assert(ints[i] == i);
  }

  assert(VST_ListPop(ints) == 9);
  assert(VST_ListLength(ints) == 9);

  VST_ListFree(ints);
  printf("\tOK\n");

  printf("Testing string concatenation: ");

  VST_String name, path1, path2, path3, full, expected, left, reappended;
  struct VST_StringView empty = VST_StringViewFromStr("");
  expected = VST_StringFromStr("home/user/models/model.obj/");
  name = VST_StringFromStr("   model.obj/    ");
  VST_StringTrimWhitespace(name);
  path1 = VST_StringFromStr("home/");
  path2 = VST_StringFromStr("user");
  path3 = VST_StringFromStr("models");
  full = VST_StringsConcatPath(path1, path2, path3, name);

  assert(VST_StringEqual(full, expected));
  assert(!VST_StringViewContainsStr(empty, "models"));
  assert(VST_StringContainsStr(full, "models"));
  
  struct VST_StringView *parts = 0;
  parts = VST_StringSplitByList(full, VST_StringViewFromStr("/"));
  reappended = VST_StringCreate();
  VST_StringJoinStringViewList(reappended, parts, VST_StringViewFromStr("/"));
  VST_ListFree(parts);
  assert(VST_StringEqual(reappended, full));

  printf("\tOK\n");

  struct VST_StringView expected_parts[] = {
      VST_StringViewFromStr("    home    "), VST_StringViewFromStr("user"),
      VST_StringViewFromStr("models"), VST_StringViewFromStr("model.obj"),
      VST_StringViewFromStr("")};

  expected_parts[0] = VST_StringViewTrimWhitespace(expected_parts[0]);

  struct VST_StringView delimiter = VST_StringViewFromStr("/");

  printf("Testing searching for substrings: ");
  usize expected_found_count = 4;
  assert(VST_StringFindAllCount(full, delimiter) == expected_found_count);

  struct VST_StringView found_views[expected_found_count];
  VST_StringFindAll(full, delimiter, found_views);

  usize expected_part_positions[] = {4, 9, 16, 26};

  for (usize i = 0; i < expected_found_count; i++) {
    assert(found_views[i].offset == expected_part_positions[i]);
  }
  printf("\tOK\n");

  printf("Testing string splitting: ");
  fflush(stdout);
  usize expected_split_count = 5;
  assert(VST_StringSplitByCount(full, delimiter) == expected_split_count);

  struct VST_StringView *split_views = VST_StringSplitByList(full, delimiter);
  assert(VST_ListCount(split_views) == expected_split_count);

  for (usize i = 0; i < VST_ListCount(split_views); i++) {
    assert(VST_StringViewEqual(split_views[i], expected_parts[i]));
  }

  VST_ListFree(split_views);

  assert(VST_StringStartsWith(full, expected_parts[0]));
  assert(!VST_StringStartsWith(full, expected_parts[1]));

  VST_StringsDestroy(name, path1, path2, path3, full, expected, reappended);
  printf("\tOK\n");


  printf("Testing parsing: ");
  fflush(stdout);  
  struct VST_StringView to_parse_doubles[] = {
      VST_StringViewFromStr("-6.3004, "),
      VST_StringViewFromStr("  +3243.5650"),
      VST_StringViewFromStr("   0"),
      VST_StringViewFromStr("12321434,54"),
  };

  double numbers_expected_doubles[] = {-6.3004, 3243.565, 0.0, 12321434};

  for (uint32_t i = 0; i < ARRLEN(to_parse_doubles); i++) {
    double result_double =
        VST_StringViewParseFloat(to_parse_doubles[i], 0, NULL);
    assert(result_double == numbers_expected_doubles[i]);
  }

  struct VST_StringView to_parse_ints[] = {
      VST_StringViewFromStr("-6.3004, "),
      VST_StringViewFromStr("  +3243.5650"),
      VST_StringViewFromStr("   0"),
      VST_StringViewFromStr("12321434,54"),
  };

  int64_t numbers_expected_ints[] = {-6.3004, 3243.565, 0.0, 12321434};

  for (uint32_t i = 0; i < ARRLEN(to_parse_ints); i++) {
    int64_t result_int = VST_StringViewParseInt(to_parse_ints[i], 0, NULL);
    assert(result_int == numbers_expected_ints[i]);
  }

  printf("\tOK\n");

  return 0;
}
