#include "vec.h"
#include "basic.h"
#include <string.h>
#include <assert.h>
#include <stdlib.h>

static Vec raw_vec_init(usize sizeof_T) {
  Vec v = {
      .ptr = NULL,
      .len = 0,
      .cap = 0,
      .size = sizeof_T,
  };
  return v;
}

static void raw_vec_free(mov(Vec *) v) {
  free(v->ptr);
  v->ptr = NULL;
}

static Vec raw_vec_clone(const brw(Vec *) v) {
  if (v->len == 0) {
    return raw_vec_init(v->size);
  }
  usize size = v->len * v->size;
  void *ptr = malloc(size);
  memcpy(ptr, v->ptr, size);
  Vec ret = {
      .ptr = ptr,
      .len = v->len,
      .cap = v->len,
      .size = v->size,
  };
  return ret;
}

static void raw_vec_clear(Vec *v) { v->len = 0; }

static void *raw_vec_index(brw(Vec *) v, usize index) {
  return index < v->len ? v->ptr + index * v->size : NULL;
}

static void raw_vec_reserve(brw(Vec *) v, usize additional) {
  usize new_cap = v->cap + additional;

  void *new_ptr = malloc(new_cap * v->size);
  assert(new_ptr != NULL);

  memcpy(new_ptr, v->ptr, v->len * v->size);
  free(v->ptr);
  v->ptr = new_ptr;
  v->cap = new_cap;
}

static void raw_vec_resize(brw(Vec *) v, usize new_size) {
  if (new_size > v->cap) {
    usize additional = max(new_size - v->cap, max(v->cap, 4));
    raw_vec_reserve(v, additional);
  }
  v->len = new_size;
}

static void raw_vec_shrink_to(brw(Vec *) v, usize new_size) {
  if (new_size >= v->cap) {
    return;
  }

  void *new_ptr = malloc(new_size * v->size);
  usize new_len = min(new_size, v->len);
  memcpy(new_ptr, v->ptr, new_len);
  free(v->ptr);
  v->ptr = new_ptr;
  v->cap = new_size;
  v->len = new_len;
}

static void raw_vec_shrink_to_fit(brw(Vec *) v) { raw_vec_shrink_to(v, v->len); }

static void *raw_vec_push(brw(Vec *) v) {
  if (v->cap > v->len) {
    void *ret = v->ptr + v->len * v->size;
    v->len++;
    return ret;
  }
  usize additional = max(v->cap, (usize)4);
  raw_vec_reserve(v, additional);
  void *ret = v->ptr + v->len * v->size;
  v->len++;
  return ret;
}

static void *raw_vec_pop(brw(Vec *) v) {
  assert(v->len != 0);

  v->len--;
  void *ret = v->ptr + v->len * v->size;
  return ret;
}

static void raw_vec_remove(brw(Vec *) v, usize index) {
  assert(index < v->len);

  if (index == v->len - 1) {
    raw_vec_pop(v);
    return;
  }

  v->len--;
  memmove(v->ptr + index * v->size, v->ptr + (index + 1) * v->size,
          (v->len - index) * v->size);
  return;
}

static void raw_vec_swap_remove(brw(Vec *) v, usize index) {
  assert(index < v->len);

  if (index == v->len - 1) {
    raw_vec_pop(v);
    return;
  }

  v->len--;
  memcpy(v->ptr + index * v->size, v->ptr + v->len * v->size, v->size);
}

const struct CVec CVec = {
    .init = raw_vec_init,

    .free = raw_vec_free,

    .clone = raw_vec_clone,

    .clear = raw_vec_clear,

    .index = raw_vec_index,

    .reserve = raw_vec_reserve,

    .resize = raw_vec_resize,

    .shrink_to = raw_vec_shrink_to,

    .shrink_to_fit = raw_vec_shrink_to_fit,

    .push = raw_vec_push,

    .pop = raw_vec_pop,

    .remove = raw_vec_remove,

    .swap_remove = raw_vec_swap_remove,

};