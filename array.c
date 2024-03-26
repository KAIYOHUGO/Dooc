#include "array.h"
#include <stdlib.h>
#include <string.h>

static Array raw_arr_init(usize size_of_T, usize len) {
  void *ptr = malloc(len * size_of_T);

  Array array = {
      .ptr = ptr,
      .len = len,
      .size = size_of_T,
  };
  return array;
}

static Array raw_arr_clone(const brw(Array *) a) {
  void *ptr = malloc(a->len * a->size);
  memcpy(ptr, a->ptr, a->len * a->size);
  Array array = {
      .ptr = ptr,
      .len = a->len,
      .size = a->size,
  };
  return array;
}

static brw(Array) raw_arr_ref(brw(void *) ptr, usize size_of_T, usize len) {
  Array array = {
      .ptr = ptr,
      .len = len,
      .size = size_of_T,
  };
  return array;
}

static Array raw_arr_empty() {
  Array array = {
      .ptr = NULL,
      .len = 0,
      .size = 0,
  };
  return array;
}

static void raw_arr_free(mov(Array *) a) {
  free(a->ptr);
  a->ptr = NULL;
}

static void *raw_arr_index(brw(Array *) a, usize index) {
  return index < a->len ? a->ptr + index * a->size : NULL;
}

const struct CArray CArray = {
    .init = raw_arr_init,

    .ref = raw_arr_ref,

    .clone = raw_arr_clone,

    .empty = raw_arr_empty,

    .free = raw_arr_free,

    .index = raw_arr_index,
};