#ifndef _CODE_CACHE_H_
#define _CODE_CACHE_H_

#include <stdbool.h>
#include <stdint.h>

typedef struct pt_code_cache_node pt_code_cache_node;

/**
   A binary search tree.
 */
struct pt_code_cache_node {

  // simulated virtual addresses this node represents
  uint32_t start;
  uint32_t end;

  // compiled code
  // TODO - fn pointer to compiled code
  
  // next nodes
  struct pt_code_cache_node* lt;
  struct pt_code_cache_node* gt;
  int height;
  int balanceFactor;
  
  // destructor
  void (*dtor)(struct pt_code_cache_node*);
  
};

/**
   @param cache Cache to add into (or NULL)
   @param item Item to add to the cache
   @return New cache with new nodes added
 */
extern pt_code_cache_node* pt_cache_add(pt_code_cache_node* cache, pt_code_cache_node* item);

/**
   Invalidates all nodes where the given address is within the start/end range.
   @param cache Cache to search in (or NULL)
   @param addr Virtual address to invalidate
   @return New cache with invalidated nodes pruned
 */
extern pt_code_cache_node* pt_cache_invalidate(pt_code_cache_node* cache, uint32_t addr);

/**
   Finds a translated block of code within the cache.
   @param cache Cache to search in (or NULL)
   @param addr Virtual address to find code for
   @return The node representing translated code at that address, or null if not present.
 */
extern pt_code_cache_node* pt_cache_find(pt_code_cache_node* cache, uint32_t addr);

#endif

