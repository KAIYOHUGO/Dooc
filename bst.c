#include "bst.h"
#include <stdlib.h>

static Bst raw_bst_init(usize size, BstCmpFn cmp) {
  Bst bst = {.root = NULL, .cmp = cmp, .size = size};
  return bst;
}

static BstNode *raw_bst_node_init(usize size) {
  BstNode *node = malloc(sizeof(BstNode) + size);
  node->parent = NULL;
  node->left = NULL;
  node->right = NULL;
  return node;
}

static void raw_bst_insert(Bst *bst, BstNode *node) {
  BstNode *parent = NULL;
  BstNode **next = &bst->root;

  while (*next != NULL) {
    parent = *next;
    if (bst->cmp((*next)->value, node->value) > 0) {
      next = &(*next)->right;
    } else {
      next = &(*next)->left;
    }
  }

  node->parent = parent;
  *next = node;
}

static BstNode *raw_bst_search(Bst *bst, const void *value) {
  BstNode *next = bst->root;

  while (next != NULL) {
    isize ord = bst->cmp(next->value, value);

    if (ord == 0) {
      return next;
    }
    if (ord > 0) {
      next = next->right;
      continue;
    }
    next = next->left;
  }
  return NULL;
}

static void bst_internal_swap(Bst *bst, BstNode *a, BstNode *b) {
  for (size_t i = 0; i < bst->size; i++) {
    u8 tmp = a->value[i];
    a->value[i] = b->value[i];
    b->value[i] = tmp;
  }
}

static BstNode *raw_bst_remove(Bst *bst, BstNode *node) {
  BstNode **parent_to_node = node->parent == NULL ? &bst->root
                             : node->parent->left == node
                                 ? &node->parent->left
                                 : &node->parent->right;

  // two child node
  // swap and remove
  if (node->left != NULL && node->right != NULL) {
    BstNode *successor = node->left;
    while (successor->right != NULL) {
      successor = successor->right;
    }
    bst_internal_swap(bst, successor, node);
    return raw_bst_remove(bst, successor);
  }

  // single child node
  // remove self
  if (node->left != NULL) {
    *parent_to_node = node->left;
    return node;
  }
  if (node->right != NULL) {
    *parent_to_node = node->right;
    return node;
  }

  // don't have child
  // remove self
  *parent_to_node = NULL;
  return node;
}

static BstNode *raw_bst_ceil_recursive(BstCmpFn cmp, BstNode *parent,
                                       const void *value) {
  if (parent == NULL)
    return NULL;

  isize ord = cmp(parent->value, value);

  if (ord == 0)
    return parent;

  if (ord > 0)
    return raw_bst_ceil_recursive(cmp, parent->right, value);

  BstNode *left = raw_bst_ceil_recursive(cmp, parent->left, value);
  return left != NULL && cmp(value, left->value) >= 0 ? left : parent;
}

static BstNode *raw_bst_ceil(Bst *bst, const void *value) {
  return raw_bst_ceil_recursive(bst->cmp, bst->root, value);
}

static BstNode *raw_bst_floor_recursive(BstCmpFn cmp, BstNode *parent,
                                        const void *value) {
  if (parent == NULL)
    return NULL;

  isize ord = cmp(parent->value, value);

  if (ord == 0)
    return parent;

  if (ord < 0)
    return raw_bst_floor_recursive(cmp, parent->left, value);

  BstNode *right = raw_bst_floor_recursive(cmp, parent->right, value);
  return right != NULL && cmp(value, right->value) <= 0 ? right : parent;
}

static BstNode *raw_bst_floor(Bst *bst, const void *value) {
  return raw_bst_floor_recursive(bst->cmp, bst->root, value);
}

const struct CBst CBst = {
    .init = raw_bst_init,
    .node_init = raw_bst_node_init,
    .insert = raw_bst_insert,
    .search = raw_bst_search,
    .remove = raw_bst_remove,
    .ceil = raw_bst_ceil,
    .floor = raw_bst_floor,
};