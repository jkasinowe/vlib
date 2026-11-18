#ifndef VSTRUCT_H
#define VSTRUCT_H
#include <stdlib.h>

void* VST_ListCreate_func(size_t element_size, size_t initial_capacity);

void VST_ListAppend_func(void **list, void *element);

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
#include <string.h>
#include <stdbool.h>
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
#endif


#endif
