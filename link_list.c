#include "link_list.h"
#include <assert.h>
#include <malloc.h>
#include <stdio.h>

static LinkListElement *raw_ll_element_init(usize size) {
  LinkListElement *ptr = malloc(sizeof(LinkListElement) + size);
  ptr->next = NULL;
  ptr->prev = NULL;
  return ptr;
}

static void raw_ll_element_free(LinkListElement *el) { free(el); }

static LinkList raw_ll_init() {
  LinkList ll = {NULL, NULL, 0};
  return ll;
}

static void raw_ll_free(LinkList *ll) {
  LinkListElement *next = ll->head;
  while (next != NULL) {
    LinkListElement *old = next;
    next = next->next;
    raw_ll_element_free(old);
  }
}

static void raw_ll_push_back(LinkList *ll, LinkListElement *ptr) {
  ++ll->len;
  ptr->prev = ll->tail;
  ptr->next = NULL;
  ll->tail = ptr;
  if (ptr->prev != NULL) {
    ptr->prev->next = ptr;
  } else {
    // empty
    ll->head = ptr;
  }
}

static void raw_ll_push_front(LinkList *ll, LinkListElement *ptr) {
  ++ll->len;
  ptr->next = ll->head;
  ptr->prev = NULL;
  ll->head = ptr;
  if (ptr->next != NULL) {
    ptr->next->prev = ptr;
  } else {
    // empty
    ll->tail = ptr;
  }
}

static LinkListElement *raw_ll_index(LinkList *ll, usize index) {
  LinkListElement *next = ll->head;
  while (index-- && next != NULL) {
    next = next->next;
  }
  return next;
}

static void raw_ll_insert(LinkList *ll, LinkListElement *el,
                          LinkListElement *new_el) {
  if (el == NULL) {
    return raw_ll_push_back(ll, new_el);
  }
  if (el->prev == NULL) {
    return raw_ll_push_front(ll, new_el);
  }
  ++ll->len;
  new_el->prev = el->prev;
  new_el->next = el;
  el->prev->next = new_el;
  el->prev = new_el;
}

static LinkListElement *raw_ll_remove(LinkList *ll, LinkListElement *item) {
  --ll->len;
  LinkListElement *prev = item->prev;
  LinkListElement *next = item->next;
  if (prev == NULL) {
    ll->head = next;
  } else {
    prev->next = next;
  }
  if (next == NULL) {
    ll->tail = prev;
  } else {
    next->prev = prev;
  }
  return item;
}

static LinkListElement *raw_ll_pop_back(LinkList *ll) {
  assert(ll->tail != NULL);

  return raw_ll_remove(ll, ll->tail);
}

static LinkListElement *raw_ll_pop_front(LinkList *ll) {
  assert(ll->tail != NULL);
  return raw_ll_remove(ll, ll->head);
}

const struct CLinkList CLinkList = {
    .element_init = raw_ll_element_init,

    .element_free = raw_ll_element_free,

    .init = raw_ll_init,

    .free = raw_ll_free,

    .push_back = raw_ll_push_back,

    .push_front = raw_ll_push_front,

    .index = raw_ll_index,

    .insert = raw_ll_insert,

    .remove = raw_ll_remove,

    .pop_back = raw_ll_pop_back,

    .pop_front = raw_ll_pop_front,
};
