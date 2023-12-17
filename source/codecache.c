#include <stddef.h>
#include "codecache.h"

// TODO - extend to being an interval tree
// https://www.geeksforgeeks.org/interval-tree/
// i.e. also store a maximum end for each subtree in each node so we can quickly figure out when to stop searching for a given address

// self-balancing AVL tree

static void update(pt_code_cache_node* node) {
  int ltHeight = -1;
  int gtHeight = -1;
  
  if(node->lt != NULL) {
    ltHeight = node->lt->height;    
  }
  if(node->gt != NULL) {
    gtHeight = node->gt->height;    
  }

  node->height = 1 + (ltHeight > gtHeight ? ltHeight : gtHeight);
  node->balanceFactor = gtHeight - ltHeight;
}

static pt_code_cache_node* rotate_left(pt_code_cache_node* a) {
  pt_code_cache_node* b = a->gt;
  if(b == NULL) {
    return a;
  }
  a->gt = b->lt;
  b->lt = a;
  update(a);
  update(b);
  return b;
}

static pt_code_cache_node* rotate_right(pt_code_cache_node* a) {
  pt_code_cache_node* b = a->lt;
  if(b == NULL) {
    return a;
  }
  a->lt = b->gt;
  b->gt = a;
  update(a);
  update(b);
  return b;
}

static pt_code_cache_node* ll(pt_code_cache_node* node) {
  return rotate_right(node);
}

static pt_code_cache_node* rr(pt_code_cache_node* node) {
  return rotate_left(node);
}

static pt_code_cache_node* lr(pt_code_cache_node* node) {
  node->lt = rotate_left(node->lt);
  return ll(node);
}

static pt_code_cache_node* rl(pt_code_cache_node* node) {
  node->gt = rotate_right(node->gt);
  return rr(node);
}

static pt_code_cache_node* balance(pt_code_cache_node* node) {
  if(node->balanceFactor == -2) {
    return (node->lt->balanceFactor) <= 0 ? ll(node) : lr(node);
  } else if(node->balanceFactor == 2) {
    return (node->gt->balanceFactor >= 0) ? rr(node) : rl(node);
  }
  return node;
}

pt_code_cache_node* pt_cache_add(pt_code_cache_node* node, pt_code_cache_node* item) {
  if(item == NULL) {
    return node;
  }
  
  if(node == NULL) {
    return item;
  }

  // if start addresses are the same, replace old node with item
  if(node->start == item->start) {
    item->lt = node->lt;
    item->gt = node->gt;
    if(node->dtor != NULL) {
      node->dtor(node);
    }
    update(item);    
    return balance(item);
  }

  if(item->start < node->start) {
    node->lt = pt_cache_add(node->lt, item);
  } else {
    node->gt = pt_cache_add(node->gt, item);
  }

  update(node);
  return balance(node);
}

