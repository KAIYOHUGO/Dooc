#if !defined(__HEAP_H)
#define __HEAP_H

#include "vec.h"

typedef bool (*HeapCmpFn)(const void *parent, const void *children);

typedef struct {
  Vec(T) list;
  HeapCmpFn cmp;
} Heap;

struct CHeap {
  Heap (*init)(usize size_of_T, HeapCmpFn cmp);

  void (*heapify_up)(Heap *heap);

  void (*heapify_down)(Heap *heap);
};

extern const struct CHeap CHeap;

#define Heap(T) Heap

#define heap_init(T, cmp) CHeap.init(sizeof(T), cmp)

#define heap_insert(T, h, v)                                                   \
  {                                                                            \
    vec_push(T, &(h)->list, v);                                                \
    CHeap.heapify_up(h);                                                       \
  }

#define heap_pop(T, h)                                                         \
  ({                                                                           \
    T ret = *vec_index(T, &(h)->list, 0);                                        \
    CVec.swap_remove(&(h)->list, 0);                                           \
    CHeap.heapify_down(h);                                                     \
    ret;                                                                       \
  })

#define heap_peak(T, h) (vec_index(T, &h->list, 0))

#endif // __HEAP_H
