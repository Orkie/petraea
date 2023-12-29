#include "struct_offsets.h"

.data
// jump table for evaluating different types of conditions
.cond_jump_table:
  .long (.cond_al - .cond_jump_table)
  .long (.cond_al - .cond_jump_table)
  .long (.cond_al - .cond_jump_table)
  .long (.cond_al - .cond_jump_table)
  .long (.cond_al - .cond_jump_table)
  .long (.cond_al - .cond_jump_table)
  .long (.cond_al - .cond_jump_table)
  .long (.cond_al - .cond_jump_table)
  .long (.cond_al - .cond_jump_table)
  .long (.cond_al - .cond_jump_table)
  .long (.cond_al - .cond_jump_table)
  .long (.cond_al - .cond_jump_table)
  .long (.cond_al - .cond_jump_table)
  .long (.cond_al - .cond_jump_table)
  .long (.cond_al - .cond_jump_table)
  .long (.cond_nv - .cond_jump_table)

.text
.global _x64_pt_arm_execute
// TODO - lahf / sahf
  // params in RCX, RDX, R8, R9 for windows
  //  RDI, RSI, RDX, RCX, R8, R9 for linux
  //  // mov OFF_pt_arm_cpu_r1(%p0), %rax


#define p0 rdi
#define p1 rsi
#define p2 rdx

// Execute instruction
// p0 = pointer to CPU structure
// p1 = type of instruction
// p2 = instruction
_x64_pt_arm_execute:
  mov %p2, %rax
  shr $28, %rax

  leaq	.cond_jump_table(%rip), %rcx
  movslq	(%rcx, %rax, 4), %rax
  addq %rcx, %rax
  jmp *%rax
evaluate_cond:
  // mov OFF_pt_arm_cpu_r1(%p0), %rax

.cond_al:
execute_instr:
  nop
  movq $0x7E110, %rax
  jmp .done

.cond_nv:
.done:
  movq $0xDEADBEEF, %rax
  ret

