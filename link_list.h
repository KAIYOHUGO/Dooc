#if !defined(__LINK_LIST_H)
#define __LINK_LIST_H

#include "basic.h"

struct LinkListElement {
  struct LinkListElement *prev, *next;
  u8 value[];
};

struct LinkList {
  struct LinkListElement *head, *tail;
  usize len;
};

typedef struct LinkListElement LinkListElement;
typedef struct LinkList LinkList;

struct CLinkList {
  LinkListElement *(*element_init)(usize size);

  void (*element_free)(LinkListElement *el);

  LinkList (*init)();

  void (*free)(LinkList *ll);

  void (*push_back)(LinkList *ll, LinkListElement *ptr);

  void (*push_front)(LinkList *ll, LinkListElement *ptr);

  LinkListElement *(*index)(LinkList *ll, usize index);

  void (*insert)(LinkList *ll, LinkListElement *el, LinkListElement *new_el);

  LinkListElement *(*remove)(LinkList *ll, LinkListElement *item);

  LinkListElement *(*pop_back)(LinkList *ll);

  LinkListElement *(*pop_front)(LinkList *ll);
};

extern const struct CLinkList CLinkList;

#define LinkList(T) LinkList

#define LinkListElement(T) LinkListElement

#define link_list_init(T) CLinkList.init()

#define link_list_free(T, ll) CLinkList.free(ll)

#define link_list_element_init(T, v)                                           \
  ({                                                                           \
    LinkListElement *el = CLinkList.element_init(sizeof(T));                   \
    *(T *)(&el->value) = v;                                                        \
    el;                                                                        \
  })

#define link_list_element_free(el) CLinkList.element_free(el)

#define link_list_element_value(T, el) (*(T *)((el)->value))

#endif // __LINK_LIST_H