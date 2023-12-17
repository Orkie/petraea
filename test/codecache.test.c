#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <stddef.h>
#include "codecache.h"

Test(pt_cache_add, can_add_to_NULL) {
  pt_code_cache_node item = {100, 200, NULL, NULL, 0, 0, NULL};
  cr_assert_eq(pt_cache_add(NULL, &item), &item);
}

Test(pt_cache_add, can_replace_self) {
  pt_code_cache_node item1 = {100, 200, NULL, NULL, 0, 0, NULL};
  pt_code_cache_node item2 = {100, 300, NULL, NULL, 0, 0, NULL};
  cr_assert_eq(pt_cache_add(&item1, &item2), &item2);
}

Test(pt_cache_add, can_append_lt) {
  pt_code_cache_node item1 = {100, 200, NULL, NULL, 0, 0, NULL};
  pt_code_cache_node item2 = {50, 300, NULL, NULL, 0, 0, NULL};

  pt_code_cache_node* result = pt_cache_add(&item1, &item2);
  cr_assert(eq(result->start, 100));
  cr_assert(eq(result->end, 200));
  
  cr_assert(not(eq(result->lt, NULL)));
  cr_assert(eq(result->gt, NULL));
  
  cr_assert(eq(result->lt->start, 50));
  cr_assert(eq(result->lt->end, 300));
}

Test(pt_cache_add, can_append_recursive) {
  pt_code_cache_node item1 = {100, 200, NULL, NULL, 0, 0, NULL};
  pt_code_cache_node item2 = {50, 300, NULL, NULL, 0, 0, NULL};
  pt_code_cache_node item3 = {25, 400, NULL, NULL, 0, 0, NULL};
  pt_code_cache_node item4 = {125, 400, NULL, NULL, 0, 0, NULL};
  
  pt_code_cache_node* result = pt_cache_add(
					    pt_cache_add(
							 pt_cache_add(&item1, &item2),
							 &item3),
					    &item4);
  cr_assert(eq(result->start, 50));
  cr_assert(eq(result->lt->start, 25));
  cr_assert(eq(result->gt->start, 100));
  cr_assert(eq(result->gt->gt->start, 125));
}


