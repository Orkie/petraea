#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <criterion/criterion.h>
#include <lightning.h>

typedef int (*pifi)(uint32_t*);

Test(jit_block, works) {
  init_jit(NULL);
  jit_state_t *_jit = jit_new_state();

  uint32_t input = 48;
  
  jit_prolog();
  jit_node_t* in = in = jit_arg();
  jit_getarg(JIT_R0, in);
  jit_ldr_ui(JIT_R1, JIT_R0);
  jit_addi(JIT_R1, JIT_R1, 1);
  jit_retr(JIT_R1);

  pifi incr = jit_emit();
  jit_clear_state();

  printf("Blah %d\n", incr(&input));

  jit_destroy_state();
  finish_jit();
}

