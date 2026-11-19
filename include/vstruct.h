#ifndef VSTRUCT_H
#define VSTRUCT_H
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>

/*

  VST_String pwd, name, full;
  VST_StringsCreate(pwd, name, full);


  VST_StringAppend(name, "vlib");
  VST_StringAppend(name, ".so");


  VST_StringAppend(pwd, "/usr/lib/");
  VST_StringEnsureLast(pwd, "/");
  VST_StringConcat(pwd, name);
  VST_StringEnsureLast(pwd, "/");


  VST_StringAppend(pwd, "%d", 5);

  VST_StringsDestroy(pwd, name);

  return full;


// all char* strings are utf8
 */


typedef struct iVST_String *VST_String;

void VST_StringEnsureEndsIn(VST_String string, char end);
void VST_StringFolderAppendCstr(VST_String string, char *folder_name);

void VST_StringTrimEnd(VST_String string);
void VST_StringTrimBegin(VST_String string);
void VST_StringTrim(VST_String string);


void VST_StringAppendFmt(VST_String string, char *fmt, ...);
void VST_StringAppendStr(VST_String string, char *str);

int VST_StringCompare(VST_String a, VST_String b);
bool VST_StringEqual(VST_String a, VST_String b);
char* VST_StringPtrGet(VST_String string);

bool VST_StringIsEmpty(VST_String string);
VST_String VST_StringCreate();
VST_String VST_StringDuplicate(VST_String string);
VST_String VST_StringFromStr(char *str);

void iVST_StringsInit(VST_String *first, ...);
void iVST_StringsDestroy(VST_String first, ...);

#define VST_StringsInit(first, ...)                                          \
  iVST_StringsInit(first, ##__VA_ARGS__, (VST_String *)-1)
#define VST_StringsDestroy(first, ...)                                         \
  iVST_StringsDestroy(first, ##__VA_ARGS__, (VST_String *)-1)



void iVST_StringsAppend(VST_String to, ...);
void iVST_StringsAppendPath(VST_String to, ...);
VST_String iVST_StringsConcat(VST_String first, ...);
VST_String iVST_StringsConcatPath(VST_String first, ...);

#define VST_StringsAppend(to, ...)                                          \
  iVST_StringsAppend(to, ##__VA_ARGS__, (VST_String) - 1)
#define VST_StringsAppendPath(to, ...)                                      \
  iVST_StringsAppendPath(to, ##__VA_ARGS__, (VST_String) - 1)


#define VST_StringsConcat(first, ...)                                          \
  iVST_StringsConcat(first, ##__VA_ARGS__, (VST_String) - 1)
#define VST_StringsConcatPath(first, ...)                                      \
  iVST_StringsConcatPath(first, ##__VA_ARGS__, (VST_String) - 1)

/* This whole thing is taken and repurposed from Sean Barrett's Nothings
   https://github.com/nothings/stb/blob/master/stb_ds.h
*/


struct iVST_ListHeader {
  size_t capacity;
  size_t count;
};

void *iVST_ListGrow_func(void* list, size_t element_size, size_t add_cap, size_t min_cap);

#define iVST_ListHeaderGet(array) (((struct iVST_ListHeader *)(array)) - 1)

#define VST_ListLength(array) ((array) ? iVST_ListHeaderGet(array)->count : 0)
#define VST_ListSize VST_ListLength
#define VST_ListCount VST_ListLength

#define VST_ListCapacity(array) ((array) ? iVST_ListHeaderGet(array)->capacity : 0)

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
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "vmath.h"
#include "vmem.h"

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
  } else if (min_cap < 4){
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
  *string = (VST_String) VMEM_Allocate(sizeof(**string), NULL);
  (*string)->ptr = (char*)VMEM_Allocate(sizeof(char), NULL);
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
    current = va_arg(strings, VST_String*);
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

void VST_StringAppendStr(VST_String string, char *str) {
  usize str_size = iVST_StrLengthGet(str);
  usize needed_size = string->start;
  needed_size += string->size;
  needed_size += str_size + 1;  
  
  if (needed_size > string->capacity) {
    string->capacity = needed_size * 2;
    string->ptr = (char*)VMEM_Reallocate(string->ptr, string->capacity);
  }


  char *copy_to_start = string->ptr + string->start + string->size;
  usize copy_to_remaining = string->capacity - string->size - string->start;

 
  if (VMEM_Copy(copy_to_start, copy_to_remaining, str, str_size)) {
    VLOG_Error("Unable to copy to string: %s", VMEM_ErrorGet());
  }
  string->size += str_size;  
  string->ptr[string->start+string->size] = '\0';
}

void VST_StringAppendFmt(VST_String string, char *fmt, ...) {
  va_list args;

  
  
  for (usize i = 0; i < strlen(fmt); i++) {
    if (fmt[i] == '%') {
      i++;
      
    }      

  }    
}


VST_String VST_StringFromStr(char *str) {
  VST_String string;
  VST_StringsInit(&string);
  VST_StringAppendStr(string, str);
  return string;
}


enum iVST_StringsAppendMode { // {"first", "second/", "third"} separator = '/'
  IVST_STRINGS_APPEND_MODE_ENSURE_BETWEEN, // output: first/second/third
  IVST_STRINGS_APPEND_MODE_JOIN_WITH,      // output: first/second//third
  IVST_STRINGS_APPEND_MODE_JOIN,           // output: firstsecond/third  
};


void iVST_StringsAppendVaList(VST_String to, va_list strings,
                                           enum iVST_StringsAppendMode mode,
                                           char symbol) {
  VST_String current = va_arg(strings, VST_String);

  VST_String dummy = (VST_String)-1;
  
  while (current != dummy) {
    if (mode == IVST_STRINGS_APPEND_MODE_ENSURE_BETWEEN) {
      VST_StringEnsureEndsIn(to, symbol);
    }
    if (mode == IVST_STRINGS_APPEND_MODE_JOIN_WITH) {
      char str[2] = {symbol, 0};
      VST_StringAppendStr(to, str);
    }      
    VST_StringAppendStr(to, VST_StringPtrGet(current));    
    current = va_arg(strings, VST_String);
  }

  va_end(strings);
}


void iVST_StringsAppend(VST_String to, ...) {
  va_list strings;
  va_start(strings, to);
  iVST_StringsAppendVaList(to, strings, IVST_STRINGS_APPEND_MODE_JOIN, 0);
}


VST_String iVST_StringsConcat(VST_String first, ...) {
  va_list strings;
  va_start(strings, first);

  VST_String result = VST_StringDuplicate(first);
  iVST_StringsAppendVaList(result, strings, IVST_STRINGS_APPEND_MODE_JOIN, 0);
  
  return result;
}

VST_String iVST_StringsConcatPath(VST_String first, ...) {
  va_list strings;
  va_start(strings, first);

  VST_String result = VST_StringDuplicate(first);
  iVST_StringsAppendVaList(result, strings, IVST_STRINGS_APPEND_MODE_ENSURE_BETWEEN, '/');
  
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

void VST_StringEnsureEndsIn(VST_String string, char end) {
  if (!VST_StringIsEmpty(string)) {
    if (VST_StringPtrGet(string)[string->size - 1] == end) {
      return;
    }
  }

  char end_str[2] = {end, 0};
  VST_StringAppendStr(string, end_str);  
}

bool VST_StringIsEmpty(VST_String string) {
  return string->size > 0 ? false : true;  
}

void iVST_StringsAppendPath(VST_String to, ...) {
  va_list strings;
  va_start(strings, to);
  iVST_StringsAppendVaList(to, strings, IVST_STRINGS_APPEND_MODE_ENSURE_BETWEEN, '/');
}

VST_String VST_StringDuplicate(VST_String string) {
  return VST_StringFromStr(VST_StringPtrGet(string));
}

void VST_StringPrint(VST_String string) {
  puts(VST_StringPtrGet(string));
}

void VST_StringTrimEnd(VST_String string) {
  while (true) {
    if (VST_StringIsEmpty(string))
      return;
    char *strptr = VST_StringPtrGet(string);
    if (!isspace(strptr[string->size - 1]))
      return;
    string->size--;
    VST_StringPtrGet(string)[string->size] = '\0';
  }
}

void VST_StringTrimBegin(VST_String string) {
  while (true) {
    if (VST_StringIsEmpty(string))
      return;
    if (!isspace(VST_StringPtrGet(string)[0]))
      return;
    string->start++;
    string->size--;
  }
}

void VST_StringTrim(VST_String string) {
  VST_StringTrimBegin(string);
  VST_StringTrimEnd(string);
}  

#endif

#endif
