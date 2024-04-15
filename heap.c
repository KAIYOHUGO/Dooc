#include "heap.h"
#include "basic.h"
#include <assert.h>
#include <stdlib.h>

static usize internal_heap_parent(usize idx) {
  assert(idx != 0);
  return (idx - 1) >> 1;
}

static usize internal_heap_left(usize idx) { return (idx << 1) + 1; }

static usize internal_heap_right(usize idx) { return (idx << 1) + 2; }

static void internal_heap_swap(Heap *heap, void *_a, void *_b) {
  assert(_a != _b);

  u8 *a = (u8 *)_a;
  u8 *b = (u8 *)_b;
  for (usize i = 0; i < heap->list.size; i++) {
    u8 tmp = a[i];
    a[i] = b[i];
    b[i] = tmp;
  }
}

static Heap raw_heap_init(usize size_of_T, HeapCmpFn cmp) {
  Heap heap = {.list = CVec.init(size_of_T), .cmp = cmp};
  return heap;
}

static void raw_heapify_up(Heap *heap) {
  usize idx = heap->list.len - 1;
  while (idx != 0) {
    usize parent_idx = internal_heap_parent(idx);
    void *parent = CVec.index(&heap->list, parent_idx);
    void *current = CVec.index(&heap->list, idx);
    if (heap->cmp(parent, current)) {
      return;
    }
    internal_heap_swap(heap, parent, current);
    idx = parent_idx;
  }
}

static void raw_heapify_down(Heap *heap) {
  usize idx = 0;
  while (true) {
    usize left_idx = internal_heap_left(idx);
    usize right_idx = internal_heap_right(idx);
    void *left = CVec.index(&heap->list, left_idx);
    void *right = CVec.index(&heap->list, right_idx);
    void *current = CVec.index(&heap->list, idx);
    if (left == NULL) {
      return;
    }
    usize child_idx;
    void *child;
    if (right == NULL || heap->cmp(left, right)) {
      child_idx = left_idx;
      child = left;
    } else {
      child_idx = right_idx;
      child = right;
    }
    if (heap->cmp(current, child)) {
      return;
    }
    internal_heap_swap(heap, current, child);
    idx = child_idx;
  }
}

const struct CHeap CHeap = {
    .init = raw_heap_init,

    .heapify_up = raw_heapify_up,

    .heapify_down = raw_heapify_down,
};