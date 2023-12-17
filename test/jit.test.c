#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <criterion/criterion.h>
#include <lightning.h>
#include <sljitLir.h>
#include "instr.h"

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

typedef sljit_sw (*func1_t)(sljit_sw a);

long int runfn(func1_t fn, pt_arm_cpu* cpu) {
  return fn(cpu);
}

Test(jit_sljit, works) {
  struct sljit_compiler* C = sljit_create_compiler(NULL, NULL);

  sljit_emit_enter(C, 0, SLJIT_ARGS1(W, P), 3, 1, 0, 0, 0);


  volatile pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r1 = 8;
  cpu.r2 = 3;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_ADD;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;

  cpu.r0 = 0x1234;
  
  pt_arm_emit_instruction(C, &cpu, &instr);
  
  
  //  sljit_emit_op1(C, SLJIT_MOV, SLJIT_R0, 0, SLJIT_S0, 0);
  //sljit_emit_op2(C, SLJIT_ADD, SLJIT_R0, 0, SLJIT_R0, 0, SLJIT_IMM, 3);

  sljit_emit_return(C, SLJIT_MOV, SLJIT_R2, 0);

  void* code = sljit_generate_code(C);

    volatile uint32_t* r= (volatile uint32_t*)&cpu.r0;
    printf("%x %x %x %x\n", r, 0, *cpu.currentRegs->regs[0], *r);

  
  func1_t func = (func1_t) code;
  runfn(func, &cpu);

  printf("%x %x %x %x\n", func(&cpu), *cpu.currentRegs->regs[0],  *cpu.currentRegs->regs[1]);
  
  sljit_free_compiler(C);
}

