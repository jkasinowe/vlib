#ifndef VSTRUCT_H
#define VSTRUCT_H
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Can be printed by specifying "%.*s" format in printf */
struct VST_StringView {
  char *ptr; // points to the already offset position
  size_t size;
  size_t offset; // offset from the start of the buffer it was created for
};

struct iVST_String; // 'opaque' structure, should not be accessed directly
typedef struct iVST_String *VST_String;

VST_String VST_StringCreate();
VST_String VST_StringDuplicate(VST_String string);

VST_String VST_StringFromStr(char *str);
VST_String VST_StringFromStringView(struct VST_StringView view);
#define VST_StringFromString(string) VST_StringDuplicate(string)

VST_String VST_StringFromSub(struct VST_StringView view, size_t start,
                             size_t size);
VST_String VST_StringFromSubString(VST_String string, size_t start,
                                   size_t size);
VST_String VST_StringFromSubStr(char *str, size_t start, size_t size);

#define VST_StringsInit(first, ...)                                            \
  iVST_StringsInit(first, ##__VA_ARGS__, (VST_String *)-1)

#define VST_StringsDestroy(first, ...)                                         \
  iVST_StringsDestroy(first, ##__VA_ARGS__, (VST_String *)-1)

// if there is no *symbol* at the end, put it there
void VST_StringEnsureEndsIn(VST_String string, struct VST_StringView in);

bool VST_StringViewEndsIn(struct VST_StringView view,
                          struct VST_StringView end);
bool VST_StringEndsIn(VST_String string, struct VST_StringView end);



bool VST_StringViewStartsWith(struct VST_StringView view,
                          struct VST_StringView start);
bool VST_StringStartsWith(VST_String string, struct VST_StringView start);


/* ..Append and ..Concat is the same, but ..Append appends to the first argument
 * while ..Concat returns the concatenation of arguments */
#define VST_StringsAppend(to, ...)                                             \
  iVST_StringsAppend(to, ##__VA_ARGS__, (VST_String) - 1)

// append strings, while ensuring there's '/' inbetween
#define VST_StringsAppendPath(to, ...)                                         \
  iVST_StringsAppendPath(to, ##__VA_ARGS__, (VST_String) - 1)

#define VST_StringsAppendEnsureBetween(between, ...)                           \
  iVST_StringsAppendEnsureBetween(between, ##__VA_ARGS__, (VST_String) - 1)

#define VST_StringsAppendJoin(with, ...)                                       \
  iVST_StringsAppendJoin(with, ##__VA_ARGS__, (VST_String) - 1)

#define VST_StringsConcat(first, ...)                                          \
  iVST_StringsConcat(first, ##__VA_ARGS__, (VST_String) - 1)

// concatenating strings, while ensuring there's '/' inbetween
#define VST_StringsConcatPath(first, ...)                                      \
  iVST_StringsConcatPath(first, ##__VA_ARGS__, (VST_String) - 1)

#define VST_StringsConcatEnsureBetween(between, ...)                           \
  iVST_StringsConcatEnsureBetween(between, ##__VA_ARGS__, (VST_String) - 1)

#define VST_StringsConcatJoin(with, ...)                                       \
  iVST_StringsConcatJoin(with, ##__VA_ARGS__, (VST_String) - 1)

void VST_StringAppendFmt(VST_String string, char *fmt, ...);
void VST_StringAppendStr(VST_String string, char *str);
void VST_StringAppendStringView(VST_String string, struct VST_StringView view);

void VST_StringTrimLeft(VST_String string, size_t amount);
void VST_StringTrimRight(VST_String string, size_t amount);
void VST_StringTrim(VST_String string, size_t left, size_t right);

struct VST_StringView VST_StringViewTrimLeft(struct VST_StringView view,
                                             size_t amount);
struct VST_StringView VST_StringViewTrimRight(struct VST_StringView view,
                                              size_t amount);
struct VST_StringView VST_StringViewTrim(struct VST_StringView view,
                                         size_t left, size_t right);

void VST_StringTrimWhitespaceLeft(VST_String string);
void VST_StringTrimWhitespaceRight(VST_String string);
void VST_StringTrimWhitespace(VST_String string);

int VST_StringCompare(VST_String a, VST_String b);
bool VST_StringEqual(VST_String a, VST_String b);
char *VST_StringPtrGet(VST_String string);

bool VST_StringIsEmpty(VST_String string);

void iVST_StringsInit(VST_String *first, ...);
void iVST_StringsDestroy(VST_String first, ...);
void iVST_StringsAppend(VST_String to, ...);
void iVST_StringsAppendPath(VST_String to, ...);
void iVST_StringsAppendJoin(struct VST_StringView with, ...);
void iVST_StringsAppendEnsureBetween(struct VST_StringView between, ...);

VST_String iVST_StringsConcat(VST_String first, ...);
VST_String iVST_StringsConcatPath(VST_String first, ...);
VST_String iVST_StringsConcatJoin(struct VST_StringView with, ...);
VST_String iVST_StringsConcatEnsureBetween(struct VST_StringView between, ...);

struct VST_StringView VST_StringViewShiftBy(struct VST_StringView *view,
                                            struct VST_StringView by);
struct VST_StringView VST_StringViewShiftByStr(struct VST_StringView *view,
                                               char *by);
struct VST_StringView VST_StringViewShiftByString(struct VST_StringView *view,
                                                  VST_String by);

size_t VST_StringViewSplitByCount(struct VST_StringView view,
                                  struct VST_StringView by);
void VST_StringViewSplitBy(struct VST_StringView view, struct VST_StringView by,
                           struct VST_StringView *parts);

size_t VST_StringSplitByCount(VST_String string, struct VST_StringView by);
void VST_StringSplitBy(VST_String string, struct VST_StringView by,
                       struct VST_StringView *parts);

size_t VST_StringViewFindAllCount(struct VST_StringView haystack,
                                  struct VST_StringView needle);

size_t VST_StringFindAllCount(VST_String haystack,
                              struct VST_StringView needle);

void VST_StringViewFindAll(struct VST_StringView haystack,
                           struct VST_StringView needle,
                           struct VST_StringView *views);
void VST_StringFindAll(VST_String haystack, struct VST_StringView needle,
                       struct VST_StringView *views);

struct VST_StringView VST_StringFindStr(VST_String haystack, char *needle);
struct VST_StringView VST_StringFind(VST_String haystack,
                                     struct VST_StringView needle);
struct VST_StringView VST_StringViewFind(struct VST_StringView haystack,
                                         struct VST_StringView needle);
struct VST_StringView VST_StringViewFindStr(struct VST_StringView haystack,
                                            char *needle);

bool VST_StringViewContains(struct VST_StringView haystack,
                            struct VST_StringView needle);
bool VST_StringViewContainsStr(struct VST_StringView haystack, char *needle);
bool VST_StringContains(VST_String haystack, struct VST_StringView needle);
bool VST_StringContainsStr(VST_String haystack, char *needle);

struct VST_StringView VST_StringViewFromSub(struct VST_StringView view,
                                            size_t start, size_t size);
struct VST_StringView VST_StringViewFromSubStr(char *str, size_t start,
                                               size_t size);
struct VST_StringView VST_StringViewFromSubString(VST_String string,
                                                  size_t start, size_t size);

struct VST_StringView VST_StringViewFromString(VST_String string);

/* Because string literals have static lifetime, the stringview created from a
 * string literal will be valid forever :33. That's really cool */
struct VST_StringView VST_StringViewFromStr(char *str);

int VST_StringViewCompare(struct VST_StringView a, struct VST_StringView b);
bool VST_StringViewEqual(struct VST_StringView a, struct VST_StringView b);

/* This dynamic array thingy is taken and repurposed from Sean Barrett's
 * Nothings https://github.com/nothings/stb/blob/master/stb_ds.h */
struct iVST_ListHeader {
  size_t capacity;
  size_t count;
};

void *iVST_ListGrow_func(void *list, size_t element_size, size_t add_cap,
                         size_t min_cap);

#define iVST_ListHeaderGet(array) (((struct iVST_ListHeader *)(array)) - 1)

#define VST_ListLength(array) ((array) ? iVST_ListHeaderGet(array)->count : 0)
#define VST_ListSize VST_ListLength
#define VST_ListCount VST_ListLength

#define VST_ListCapacity(array)                                                \
  ((array) ? iVST_ListHeaderGet(array)->capacity : 0)

#define iVST_ListMaybeGrow(array, added_cap)                                   \
  (!(array) || VST_ListLength(array) + (added_cap) >= VST_ListCapacity(array)  \
       ? (iVST_ListGrow(array, added_cap, 0), 0)                               \
       : 0)

#define iVST_ListGrow(array, added_cap, min_cap)                               \
  ((array) =                                                                   \
       iVST_ListGrow_func((array), sizeof *(array), (added_cap), (min_cap)))

#define VST_ListPush(array, element)                                           \
  (iVST_ListMaybeGrow(array, 1), (array)[VST_ListLength(array)] = (element),   \
   (array)[iVST_ListHeaderGet(array)->count++])

#define VST_ListFree(array) (free(iVST_ListHeaderGet(array)))

#define VSTRUCT_IMPLEMENTATION
#ifdef VSTRUCT_IMPLEMENTATION
#include "vmath.h"
#include "vmem.h"
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

void *iVST_ListGrow_func(void *list, size_t element_size, size_t add_cap,
                         size_t min_cap) {
  void *list2;
  size_t min_len = VST_ListLength(list) + add_cap;

  if (min_len > min_cap) {
    min_cap = min_len;
  }

  if (min_cap <= VST_ListCapacity(list)) {
    return list; // no need to grow
  }

  if (min_cap < 2 * VST_ListCapacity(list)) {
    min_cap = 2 * VST_ListCapacity(list);
  } else if (min_cap < 4) {
    min_cap = 4;
  }

  list2 = (struct iVST_ListHeader *)realloc(
      (list) ? (void *)iVST_ListHeaderGet(list) : NULL,
      element_size * min_cap + sizeof(struct iVST_ListHeader));

  list2 = (char *)list2 + sizeof(struct iVST_ListHeader);

  if (list == NULL) {
    iVST_ListHeaderGet(list2)->count = 0;
  }

  iVST_ListHeaderGet(list2)->capacity = min_cap;

  return list2;
}

struct iVST_String {
  usize size;
  usize start;
  char *ptr;
  usize capacity;
};

void iVST_StringInit(VST_String *string) {
  *string = (VST_String)VMEM_Allocate(sizeof(**string), NULL);
  (*string)->ptr = (char *)VMEM_Allocate(sizeof(char), NULL);
  (*string)->size = 0;
  (*string)->start = 0;
  (*string)->ptr[0] = '\0';
}

void iVST_StringsInit(VST_String *first, ...) {
  va_list strings;

  va_start(strings, first);

  VST_String *current = first;

  while (current != (VST_String *)-1) {
    iVST_StringInit(current);
    current = va_arg(strings, VST_String *);
  }

  va_end(strings);
}

void iVST_StringDestroy(VST_String string) {
  if (!string)
    return;

  free(string->ptr);
  free(string);
}

void iVST_StringsDestroy(VST_String first, ...) {
  va_list strings;

  va_start(strings, first);

  VST_String current = first;

  while (current != (VST_String)-1) {
    iVST_StringDestroy(current);
    current = va_arg(strings, VST_String);
  }

  va_end(strings);
}

usize iVST_StrLengthGet(char *str) {
  usize i = 0;
  while (str[i] != '\0') {
    i++;
  }
  return i;
}

void VST_StringAppendStringView(VST_String string, struct VST_StringView view) {
  usize str_size = view.size;
  usize needed_size = string->start;
  needed_size += string->size;
  needed_size += str_size + 1;

  if (needed_size > string->capacity) {
    string->capacity = needed_size * 2;
    string->ptr = (char *)VMEM_Reallocate(string->ptr, string->capacity);
  }

  char *copy_to_start = string->ptr + string->start + string->size;
  usize copy_to_remaining = string->capacity - string->size - string->start;

  if (VMEM_Copy(copy_to_start, copy_to_remaining, view.ptr, str_size)) {
    VLOG_Error("Unable to copy to string: %s", VMEM_ErrorGet());
  }
  string->size += str_size;
  string->ptr[string->start + string->size] = '\0';
}

void VST_StringAppendStr(VST_String string, char *str) {
  VST_StringAppendStringView(string, VST_StringViewFromStr(str));
}

void VST_StringAppendFmt(VST_String string, char *fmt, ...) {
  va_list args;

  for (usize i = 0; i < strlen(fmt); i++) {
    if (fmt[i] == '%') {
      i++;
    }
  }
}

VST_String VST_StringFromStringView(struct VST_StringView view) {
  VST_String string = VST_StringCreate();
  VST_StringAppendStringView(string, view);
  return string;
}

VST_String VST_StringFromStr(char *str) {
  return VST_StringFromStringView(VST_StringViewFromStr(str));
}

enum iVST_StringsAppendMode { // {"first", "second/", "third"} separator = '/'
  IVST_STRINGS_APPEND_MODE_ENSURE_BETWEEN, // output: first/second/third
  IVST_STRINGS_APPEND_MODE_JOIN_WITH,      // output: first/second//third
  IVST_STRINGS_APPEND_MODE_JOIN,           // output: firstsecond/third
};

void iVST_StringsAppendVaList(VST_String to, va_list strings,
                              enum iVST_StringsAppendMode mode,
                              struct VST_StringView with) {
  VST_String current = va_arg(strings, VST_String);

  VST_String dummy = (VST_String)-1;

  while (current != dummy) {
    if (mode == IVST_STRINGS_APPEND_MODE_ENSURE_BETWEEN) {
      VST_StringEnsureEndsIn(to, with);
    }
    if (mode == IVST_STRINGS_APPEND_MODE_JOIN_WITH) {
      VST_StringAppendStringView(to, with);
    }
    VST_StringAppendStr(to, VST_StringPtrGet(current));
    current = va_arg(strings, VST_String);
  }

  va_end(strings);
}

void iVST_StringsAppend(VST_String to, ...) {
  va_list strings;
  va_start(strings, to);
  iVST_StringsAppendVaList(to, strings, IVST_STRINGS_APPEND_MODE_JOIN,
                           (struct VST_StringView){0});
}

VST_String iVST_StringsConcat(VST_String first, ...) {
  va_list strings;
  va_start(strings, first);

  VST_String result = VST_StringDuplicate(first);
  iVST_StringsAppendVaList(result, strings, IVST_STRINGS_APPEND_MODE_JOIN,
                           (struct VST_StringView){0});

  return result;
}

VST_String iVST_StringsConcatJoin(struct VST_StringView with, ...) {
  va_list strings;
  va_start(strings, with);
  VST_String result = VST_StringDuplicate(va_arg(strings, VST_String));
  iVST_StringsAppendVaList(result, strings, IVST_STRINGS_APPEND_MODE_JOIN_WITH,
                           with);
  return result;
}

VST_String iVST_StringsConcatEnsureBetween(struct VST_StringView between, ...) {
  va_list strings;
  va_start(strings, between);
  VST_String result = VST_StringDuplicate(va_arg(strings, VST_String));
  iVST_StringsAppendVaList(result, strings,
                           IVST_STRINGS_APPEND_MODE_ENSURE_BETWEEN, between);
  return result;
}

VST_String iVST_StringsConcatPath(VST_String first, ...) {
  va_list strings;
  va_start(strings, first);

  VST_String result = VST_StringDuplicate(first);
  struct VST_StringView delimiter = VST_StringViewFromStr("/");

  iVST_StringsAppendVaList(result, strings,
                           IVST_STRINGS_APPEND_MODE_ENSURE_BETWEEN, delimiter);

  return result;
}

int VST_StringCompare(VST_String a, VST_String b) {
  if (a->size != b->size) {
    return a->size - b->size;
  }

  char *a_ptr = VST_StringPtrGet(a);
  char *b_ptr = VST_StringPtrGet(b);
  for (usize i = 0; i < a->size; i++) {
    if (a_ptr[i] != b_ptr[i]) {
      return a_ptr[i] - b_ptr[i];
    }
  }
  return 0;
}

bool VST_StringEqual(VST_String a, VST_String b) {
  return !VST_StringCompare(a, b);
}

char *VST_StringPtrGet(VST_String string) {
  return string->ptr + string->start;
}

VST_String VST_StringCreate() {
  VST_String string;
  VST_StringsInit(&string);
  return string;
}

bool VST_StringViewEndsIn(struct VST_StringView view,
                          struct VST_StringView end) {
  if (view.size < end.size)
    return false;
  if (end.size == 0) {
    if (view.size == 0)
      return true;
    return false;
  }
  if (!VST_StringViewContains(view, end))
    return false;

  struct VST_StringView found = VST_StringViewFind(view, end);
  if (found.offset != view.offset + view.size - end.size)
    return false;

  return true;
}

bool VST_StringEndsIn(VST_String string, struct VST_StringView end) {
  return VST_StringViewEndsIn(VST_StringViewFromString(string), end);
}

void VST_StringEnsureEndsIn(VST_String string, struct VST_StringView end) {
  if (VST_StringEndsIn(string, end))
    return;

  VST_StringAppendStringView(string, end);
}

bool VST_StringViewStartsWith(struct VST_StringView view,
                          struct VST_StringView start) {
  if (view.size < start.size)
    return false;
  if (start.size == 0) {
    if (view.size == 0)
      return true;
    return false;
  }
  if (!VST_StringViewContains(view, start))
    return false;

  struct VST_StringView found = VST_StringViewFind(view, start);
  if (found.offset != view.offset)
    return false;

  return true;
}

bool VST_StringStartsWith(VST_String string, struct VST_StringView start) {
  return VST_StringViewStartsWith(VST_StringViewFromString(string), start);
}


bool VST_StringIsEmpty(VST_String string) {
  return string->size > 0 ? false : true;
}

void iVST_StringsAppendPath(VST_String to, ...) {
  va_list strings;
  va_start(strings, to);

  struct VST_StringView delimiter = VST_StringViewFromStr("/");
  iVST_StringsAppendVaList(to, strings, IVST_STRINGS_APPEND_MODE_ENSURE_BETWEEN,
                           delimiter);
}

VST_String VST_StringDuplicate(VST_String string) {
  return VST_StringFromStr(VST_StringPtrGet(string));
}

void VST_StringPrint(VST_String string) { puts(VST_StringPtrGet(string)); }

struct VST_StringView VST_StringViewTrimLeft(struct VST_StringView view,
                                             size_t amount) {
  usize result_size = view.size > amount ? view.size - amount : 0;
  return VST_StringViewFromSub(view, amount, result_size);
}

struct VST_StringView VST_StringViewTrimRight(struct VST_StringView view,
                                              size_t amount) {
  usize result_size = view.size > amount ? view.size - amount : 0;
  return VST_StringViewFromSub(view, 0, amount);
}

struct VST_StringView VST_StringViewTrim(struct VST_StringView view,
                                         size_t left, size_t right) {
  usize result_size = view.size > right ? view.size - right : 0;
  return VST_StringViewFromSub(view, left, result_size);
}

void VST_StringTrim(VST_String string, size_t left, size_t right) {
  VST_StringTrimLeft(string, left);
  VST_StringTrimRight(string, right);
}

void VST_StringTrimLeft(VST_String string, size_t amount) {
  for (usize i = 0; i < amount; i++) {
    if (VST_StringIsEmpty(string))
      return;
    string->start++;
    string->size--;
  }
}

void VST_StringTrimRight(VST_String string, size_t amount) {
  for (usize i = 0; i < amount; i++) {
    if (VST_StringIsEmpty(string))
      return;
    string->size--;
    VST_StringPtrGet(string)[string->size] = '\0';
  }
}

void VST_StringTrimWhitespaceRight(VST_String string) {
  while (true) {
    if (VST_StringIsEmpty(string))
      return;
    char *strptr = VST_StringPtrGet(string);
    if (!isspace(strptr[string->size - 1]))
      return;
    VST_StringTrimRight(string, 1);
  }
}

void VST_StringTrimWhitespaceLeft(VST_String string) {
  while (true) {
    if (VST_StringIsEmpty(string))
      return;
    if (!isspace(VST_StringPtrGet(string)[0]))
      return;
    VST_StringTrimLeft(string, 1);
  }
}

void VST_StringTrimWhitespace(VST_String string) {
  VST_StringTrimWhitespaceLeft(string);
  VST_StringTrimWhitespaceRight(string);
}

struct VST_StringView VST_StringViewFind(struct VST_StringView haystack,
                                         struct VST_StringView needle) {
  if (needle.size > haystack.size) {
    return (struct VST_StringView){0};
  }
  for (usize i = 0; i < haystack.size - needle.size + 1; i++) {
    struct VST_StringView haystack_view =
        VST_StringViewFromSub(haystack, i, needle.size);
    if (VST_StringViewEqual(haystack_view, needle))
      return haystack_view;
  }

  return (struct VST_StringView){0};
}

struct VST_StringView VST_StringViewFindStr(struct VST_StringView haystack,
                                            char *needle) {
  return VST_StringViewFind(haystack, VST_StringViewFromStr(needle));
}

struct VST_StringView VST_StringFind(VST_String haystack,
                                     struct VST_StringView needle) {
  return VST_StringViewFind(VST_StringViewFromString(haystack), needle);
}

struct VST_StringView VST_StringFindStr(VST_String haystack, char *needle) {
  return VST_StringViewFind(VST_StringViewFromString(haystack),
                            VST_StringViewFromStr(needle));
}

struct VST_StringView VST_StringViewFromSub(struct VST_StringView view,
                                            size_t start, size_t size) {
  struct VST_StringView result = {0};
  result.size = size;
  result.offset = view.offset + start;
  result.ptr = view.ptr + start;
  return result;
}

struct VST_StringView VST_StringViewFromSubString(VST_String string,
                                                  size_t start, size_t size) {
  return VST_StringViewFromSub(VST_StringViewFromString(string), start, size);
}

struct VST_StringView VST_StringViewFromSubStr(char *str, size_t start,
                                               size_t size) {
  return VST_StringViewFromSub(VST_StringViewFromStr(str), start, size);
}

int VST_StringViewCompare(struct VST_StringView a, struct VST_StringView b) {
  if (a.size != b.size) {
    return a.size - b.size;
  }

  char *a_ptr = a.ptr;
  char *b_ptr = b.ptr;
  for (usize i = 0; i < a.size; i++) {
    if (a_ptr[i] != b_ptr[i]) {
      return a_ptr[i] - b_ptr[i];
    }
  }
  return 0;
}

bool VST_StringViewEqual(struct VST_StringView a, struct VST_StringView b) {
  return !VST_StringViewCompare(a, b);
}

struct VST_StringView VST_StringViewFromString(VST_String string) {
  struct VST_StringView result = {0};
  result.offset = 0;
  result.size = string->size;
  result.ptr = VST_StringPtrGet(string);

  return result;
}

struct VST_StringView VST_StringViewFromStr(char *str) {
  struct VST_StringView result = {0};
  result.offset = 0;
  result.ptr = str;
  result.size = iVST_StrLengthGet(str);

  return result;
}

bool VST_StringViewContains(struct VST_StringView haystack,
                            struct VST_StringView needle) {
  return VST_StringViewEqual(VST_StringViewFind(haystack, needle), needle);
}

bool VST_StringViewContainsStr(struct VST_StringView haystack, char *needle) {
  return VST_StringViewContains(haystack, VST_StringViewFromStr(needle));
}

bool VST_StringContains(VST_String haystack, struct VST_StringView needle) {
  return VST_StringViewContains(VST_StringViewFromString(haystack), needle);
}

bool VST_StringContainsStr(VST_String haystack, char *str) {
  return VST_StringContains(haystack, VST_StringViewFromStr(str));
}

VST_String VST_StringShiftBy(VST_String string, struct VST_StringView by) {
  struct VST_StringView found = VST_StringFind(string, by);

  VST_String left = VST_StringFromSubString(string, 0, found.offset);
  VST_StringTrimLeft(string, found.offset + found.size);

  return left;
}

VST_String VST_StringShiftByStr(VST_String string, char *by) {
  return VST_StringShiftBy(string, VST_StringViewFromStr(by));
}

VST_String VST_StringShiftByString(VST_String string, VST_String by) {
  return VST_StringShiftBy(string, VST_StringViewFromString(by));
}

struct VST_StringView VST_StringViewShiftAt(struct VST_StringView *view,
                                            size_t index) {
  struct VST_StringView left = VST_StringViewFromSub(*view, 0, index);
  *view = VST_StringViewTrimLeft(*view, index);
  return left;
}

struct VST_StringView VST_StringViewShiftBy(struct VST_StringView *view,
                                            struct VST_StringView by) {
  struct VST_StringView found = VST_StringViewFind(*view, by);
  struct VST_StringView left =
      VST_StringViewFromSub(*view, 0, found.offset - view->offset);
  *view =
      VST_StringViewTrimLeft(*view, found.offset + found.size - view->offset);
  return left;
}

struct VST_StringView VST_StringViewShiftByStr(struct VST_StringView *view,
                                               char *by) {
  return VST_StringViewShiftBy(view, VST_StringViewFromStr(by));
}
struct VST_StringView VST_StringViewShiftByString(struct VST_StringView *view,
                                                  VST_String by) {
  return VST_StringViewShiftBy(view, VST_StringViewFromString(by));
}

size_t VST_StringViewSplitByCount(struct VST_StringView view,
                                  struct VST_StringView by) {
  return VST_StringViewFindAllCount(view, by) + 1;
}

void VST_StringViewSplitBy(struct VST_StringView view, struct VST_StringView by,
                           struct VST_StringView *parts) {
  usize count = VST_StringViewSplitByCount(view, by);

  for (usize i = 0; i < count - 1; i++) {
    parts[i] = VST_StringViewShiftBy(&view, by);
  }

  parts[count - 1] = view;
}

size_t VST_StringSplitByCount(VST_String string, struct VST_StringView by) {
  return VST_StringViewSplitByCount(VST_StringViewFromString(string), by);
}
void VST_StringSplitBy(VST_String string, struct VST_StringView by,
                       struct VST_StringView *parts) {
  VST_StringViewSplitBy(VST_StringViewFromString(string), by, parts);
}

VST_String VST_StringFromSub(struct VST_StringView view, size_t start,
                             size_t size) {
  struct VST_StringView temp = VST_StringViewFromSub(view, start, size);
  return VST_StringFromStringView(temp);
}

VST_String VST_StringFromSubString(VST_String string, size_t start,
                                   size_t size) {
  return VST_StringFromSub(VST_StringViewFromString(string), start, size);
}

VST_String VST_StringFromSubStr(char *str, size_t start, size_t size) {
  return VST_StringFromSub(VST_StringViewFromStr(str), start, size);
}

size_t VST_StringViewFindAllCount(struct VST_StringView haystack,
                                  struct VST_StringView needle) {
  struct VST_StringView found = VST_StringViewFind(haystack, needle);
  if (!VST_StringViewEqual(found, needle))
    return 0;
  size_t count = 0;

  while (VST_StringViewEqual(found, needle)) {
    count++;
    haystack = VST_StringViewTrimLeft(haystack, found.offset + found.size -
                                                    haystack.offset);
    found = VST_StringViewFind(haystack, needle);
  }

  return count;
}

void VST_StringViewFindAll(struct VST_StringView haystack,
                           struct VST_StringView needle,
                           struct VST_StringView *views) {
  usize count = VST_StringViewFindAllCount(haystack, needle);
  for (usize i = 0; i < count; i++) {
    views[i] = VST_StringViewFind(haystack, needle);
    haystack = VST_StringViewTrimLeft(
        haystack, views[i].offset + views[i].size - haystack.offset);
  }
}

void VST_StringFindAll(VST_String haystack, struct VST_StringView needle,
                       struct VST_StringView *views) {
  return VST_StringViewFindAll(VST_StringViewFromString(haystack), needle,
                               views);
}

size_t VST_StringFindAllCount(VST_String haystack,
                              struct VST_StringView needle) {
  return VST_StringViewFindAllCount(VST_StringViewFromString(haystack), needle);
}

#endif

#endif
