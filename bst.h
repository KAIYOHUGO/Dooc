#if !defined(__BST_H)
#define __BST_H
#include "basic.h"

// return >0 if should at right, =0 if equal
typedef isize (*BstCmpFn)(const void *parent, const void *child);

struct BstNode {
  // right (larger)
  // left (less equal)
  struct BstNode *parent, *left, *right;
  u8 value[];
};

typedef struct BstNode BstNode;

typedef struct {
  BstNode *root;
  BstCmpFn cmp;
  usize size;
} Bst;

struct CBst {
  Bst (*init)(usize size, BstCmpFn cmp);
  BstNode *(*node_init)(usize size);
  void (*insert)(Bst *bst, BstNode *node);
  BstNode *(*search)(Bst *bst, const void *value);
  BstNode *(*remove)(Bst *bst, BstNode *node);
  BstNode *(*ceil)(Bst *bst, const void *value);
  BstNode *(*floor)(Bst *bst, const void *value);
};

extern const struct CBst CBst;

#define Bst(T) Bst

#define bst_init(T, cmp) (CBst.init(sizeof(T), cmp))

#define bst_node_init(T, v)                                                    \
  ({                                                                           \
    BstNode *ret = CBst.node_init(sizeof(T));                                  \
    *(T *)ret->value = v;                                                      \
    ret;                                                                       \
  })

#define bst_node_value(T, el) (*(T *)((el)->value))

#endif