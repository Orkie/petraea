#include <stdlib.h>
#include <stdio.h>
#include <criterion/criterion.h>
#include "cpu.h"
#include "instr.h"

///////////////////////////////////////////
// Evaluate operand 2
///////////////////////////////////////////

Test(eval_operand2, can_evaluate_immediate_value) {
  __arm_cpu cpu;
  arm_init_cpu(&cpu, NULL, NULL, NULL);
  bool carryValid;
  bool carry;

  __arm_operand2 op;
  op.is_immediate = true;
  op.op.imm.value = 0x123456;
  
  uint32_t result = eval_operand2(&cpu, &op, &carryValid, &carry);

  cr_assert_eq(result, 0x123456);
  cr_assert_eq(carryValid, true);
  cr_assert_eq(carry, false);
}

Test(eval_operand2, can_evaluate_register_lsl0) {
  __arm_cpu cpu;
  arm_init_cpu(&cpu, NULL, NULL, NULL);
  bool carryValid;
  bool carry;

  __arm_operand2 op;
  op.is_immediate = false;
  op.op.reg.reg = REG_R4;
  op.op.reg.is_register_shift = false;
  op.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  op.op.reg.shift_imm = 0;

  cpu.r4 = 0x123456;
  
  uint32_t result = eval_operand2(&cpu, &op, &carryValid, &carry);

  cr_assert_eq(result, 0x123456);
  cr_assert_eq(carryValid, false);
  cr_assert_eq(carry, false);
}

Test(eval_operand2, can_evaluate_register_lsl_r3_no_carry) {
  __arm_cpu cpu;
  arm_init_cpu(&cpu, NULL, NULL, NULL);
  bool carryValid;
  bool carry;

  __arm_operand2 op;
  op.is_immediate = false;
  op.op.reg.reg = REG_R4;
  op.op.reg.is_register_shift = true;
  op.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  op.op.reg.shift_reg = REG_R3;

  cpu.r3 = 4;
  cpu.r4 = 0x123456;
  
  uint32_t result = eval_operand2(&cpu, &op, &carryValid, &carry);

  cr_assert_eq(result, 0x1234560);
  cr_assert_eq(carryValid, true);
  cr_assert_eq(carry, false);
}

Test(eval_operand2, can_evaluate_register_lsl_r3_with_carry) {
  __arm_cpu cpu;
  arm_init_cpu(&cpu, NULL, NULL, NULL);
  bool carryValid;
  bool carry;

  __arm_operand2 op;
  op.is_immediate = false;
  op.op.reg.reg = REG_R4;
  op.op.reg.is_register_shift = true;
  op.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  op.op.reg.shift_reg = REG_R3;

  cpu.r3 = 4;
  cpu.r4 = 0xFFFFFFFF;
  
  uint32_t result = eval_operand2(&cpu, &op, &carryValid, &carry);

  cr_assert_eq(result, 0xFFFFFFF0);
  cr_assert_eq(carryValid, true);
  cr_assert_eq(carry, true);
}

Test(eval_operand2, can_evaluate_register_lsl_r3_where_r3_is_0) {
  __arm_cpu cpu;
  arm_init_cpu(&cpu, NULL, NULL, NULL);
  bool carryValid;
  bool carry;

  __arm_operand2 op;
  op.is_immediate = false;
  op.op.reg.reg = REG_R4;
  op.op.reg.is_register_shift = true;
  op.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  op.op.reg.shift_reg = REG_R3;

  cpu.r3 = 0;
  cpu.r4 = 0x123456;
  
  uint32_t result = eval_operand2(&cpu, &op, &carryValid, &carry);

  cr_assert_eq(result, 0x123456);
  cr_assert_eq(carryValid, true);
  cr_assert_eq(carry, false);
}

Test(eval_operand2, can_evaluate_register_rsr0) {
  __arm_cpu cpu;
  arm_init_cpu(&cpu, NULL, NULL, NULL);
  bool carryValid;
  bool carry;

  __arm_operand2 op;
  op.is_immediate = false;
  op.op.reg.reg = REG_R4;
  op.op.reg.is_register_shift = false;
  op.op.reg.shift_type = SHIFT_LOGICAL_RIGHT;
  op.op.reg.shift_imm = 0;

  cpu.r4 = 0x80000000;
  
  uint32_t result = eval_operand2(&cpu, &op, &carryValid, &carry);

  cr_assert_eq(result, 0x0);
  cr_assert_eq(carryValid, true);
  cr_assert_eq(carry, true);
}

Test(eval_operand2, can_evaluate_register_rsr_r3_no_carry) {
  __arm_cpu cpu;
  arm_init_cpu(&cpu, NULL, NULL, NULL);
  bool carryValid;
  bool carry;

  __arm_operand2 op;
  op.is_immediate = false;
  op.op.reg.reg = REG_R4;
  op.op.reg.is_register_shift = true;
  op.op.reg.shift_type = SHIFT_LOGICAL_RIGHT;
  op.op.reg.shift_reg = REG_R3;

  cpu.r3 = 4;
  cpu.r4 = 0x1234560;
  
  uint32_t result = eval_operand2(&cpu, &op, &carryValid, &carry);

  cr_assert_eq(result, 0x123456);
  cr_assert_eq(carryValid, true);
  cr_assert_eq(carry, false);
}

Test(eval_operand2, can_evaluate_register_rsr_r3_with_carry) {
  __arm_cpu cpu;
  arm_init_cpu(&cpu, NULL, NULL, NULL);
  bool carryValid;
  bool carry;

  __arm_operand2 op;
  op.is_immediate = false;
  op.op.reg.reg = REG_R4;
  op.op.reg.is_register_shift = true;
  op.op.reg.shift_type = SHIFT_LOGICAL_RIGHT;
  op.op.reg.shift_reg = REG_R3;

  cpu.r3 = 4;
  cpu.r4 = 0xFFFFFFFF;
  
  uint32_t result = eval_operand2(&cpu, &op, &carryValid, &carry);

  cr_assert_eq(result, 0x0FFFFFFF);
  cr_assert_eq(carryValid, true);
  cr_assert_eq(carry, true);
}

Test(eval_operand2, can_evaluate_register_rsr_r3_where_r3_is_0) {
  __arm_cpu cpu;
  arm_init_cpu(&cpu, NULL, NULL, NULL);
  bool carryValid;
  bool carry;

  __arm_operand2 op;
  op.is_immediate = false;
  op.op.reg.reg = REG_R4;
  op.op.reg.is_register_shift = true;
  op.op.reg.shift_type = SHIFT_LOGICAL_RIGHT;
  op.op.reg.shift_reg = REG_R3;

  cpu.r3 = 0;
  cpu.r4 = 0x123456;
  
  uint32_t result = eval_operand2(&cpu, &op, &carryValid, &carry);

  cr_assert_eq(result, 0x123456);
  cr_assert_eq(carryValid, true);
  cr_assert_eq(carry, false);
}

/////

Test(eval_operand2, can_evaluate_register_asr0_positive) {
  __arm_cpu cpu;
  arm_init_cpu(&cpu, NULL, NULL, NULL);
  bool carryValid;
  bool carry;

  __arm_operand2 op;
  op.is_immediate = false;
  op.op.reg.reg = REG_R4;
  op.op.reg.is_register_shift = false;
  op.op.reg.shift_type = SHIFT_ARITHMETIC_RIGHT;
  op.op.reg.shift_imm = 0;

  cpu.r4 = 0x80000000;
  
  uint32_t result = eval_operand2(&cpu, &op, &carryValid, &carry);

  cr_assert_eq(result, 0xFFFFFFFF);
  cr_assert_eq(carryValid, true);
  cr_assert_eq(carry, true);
}

Test(eval_operand2, can_evaluate_register_asr0_negative) {
  __arm_cpu cpu;
  arm_init_cpu(&cpu, NULL, NULL, NULL);
  bool carryValid;
  bool carry;

  __arm_operand2 op;
  op.is_immediate = false;
  op.op.reg.reg = REG_R4;
  op.op.reg.is_register_shift = false;
  op.op.reg.shift_type = SHIFT_ARITHMETIC_RIGHT;
  op.op.reg.shift_imm = 0;

  cpu.r4 = 0x00000000;
  
  uint32_t result = eval_operand2(&cpu, &op, &carryValid, &carry);

  cr_assert_eq(result, 0x0);
  cr_assert_eq(carryValid, true);
  cr_assert_eq(carry, false);
}

Test(eval_operand2, can_evaluate_register_asr_r3_no_carry_positive) {
  __arm_cpu cpu;
  arm_init_cpu(&cpu, NULL, NULL, NULL);
  bool carryValid;
  bool carry;

  __arm_operand2 op;
  op.is_immediate = false;
  op.op.reg.reg = REG_R4;
  op.op.reg.is_register_shift = true;
  op.op.reg.shift_type = SHIFT_ARITHMETIC_RIGHT;
  op.op.reg.shift_reg = REG_R3;

  cpu.r3 = 4;
  cpu.r4 = 0x123450;
  
  uint32_t result = eval_operand2(&cpu, &op, &carryValid, &carry);

  cr_assert_eq(result, 0x12345);
  cr_assert_eq(carryValid, true);
  cr_assert_eq(carry, false);
}

Test(eval_operand2, can_evaluate_register_asr_r3_no_carry_negative) {
  __arm_cpu cpu;
  arm_init_cpu(&cpu, NULL, NULL, NULL);
  bool carryValid;
  bool carry;

  __arm_operand2 op;
  op.is_immediate = false;
  op.op.reg.reg = REG_R4;
  op.op.reg.is_register_shift = true;
  op.op.reg.shift_type = SHIFT_ARITHMETIC_RIGHT;
  op.op.reg.shift_reg = REG_R3;

  cpu.r3 = 4;
  cpu.r4 = 0x80123450;
  
  uint32_t result = eval_operand2(&cpu, &op, &carryValid, &carry);

  cr_assert_eq(result, 0xF8012345);
  cr_assert_eq(carryValid, true);
  cr_assert_eq(carry, false);
}

Test(eval_operand2, can_evaluate_register_asr_r3_with_carry_positive) {
  __arm_cpu cpu;
  arm_init_cpu(&cpu, NULL, NULL, NULL);
  bool carryValid;
  bool carry;

  __arm_operand2 op;
  op.is_immediate = false;
  op.op.reg.reg = REG_R4;
  op.op.reg.is_register_shift = true;
  op.op.reg.shift_type = SHIFT_ARITHMETIC_RIGHT;
  op.op.reg.shift_reg = REG_R3;

  cpu.r3 = 4;
  cpu.r4 = 0x0FFFFFFF;
  
  uint32_t result = eval_operand2(&cpu, &op, &carryValid, &carry);

  cr_assert_eq(result, 0x00FFFFFF);
  cr_assert_eq(carryValid, true);
  cr_assert_eq(carry, true);
}

Test(eval_operand2, can_evaluate_register_asr_r3_with_carry_negative) {
  __arm_cpu cpu;
  arm_init_cpu(&cpu, NULL, NULL, NULL);
  bool carryValid;
  bool carry;

  __arm_operand2 op;
  op.is_immediate = false;
  op.op.reg.reg = REG_R4;
  op.op.reg.is_register_shift = true;
  op.op.reg.shift_type = SHIFT_ARITHMETIC_RIGHT;
  op.op.reg.shift_reg = REG_R3;

  cpu.r3 = 4;
  cpu.r4 = 0xFFFFFFFF;
  
  uint32_t result = eval_operand2(&cpu, &op, &carryValid, &carry);

  cr_assert_eq(result, 0xFFFFFFFF);
  cr_assert_eq(carryValid, true);
  cr_assert_eq(carry, true);
}

Test(eval_operand2, can_evaluate_register_asr_r3_where_r3_is_0) {
  __arm_cpu cpu;
  arm_init_cpu(&cpu, NULL, NULL, NULL);
  bool carryValid;
  bool carry;

  __arm_operand2 op;
  op.is_immediate = false;
  op.op.reg.reg = REG_R4;
  op.op.reg.is_register_shift = true;
  op.op.reg.shift_type = SHIFT_ARITHMETIC_RIGHT;
  op.op.reg.shift_reg = REG_R3;

  cpu.r3 = 0;
  cpu.r4 = 0x123456;
  
  uint32_t result = eval_operand2(&cpu, &op, &carryValid, &carry);

  cr_assert_eq(result, 0x123456);
  cr_assert_eq(carryValid, true);
  cr_assert_eq(carry, false);
}

///////////////////////////////////////////
// Branch
///////////////////////////////////////////

Test(executor_branch, can_execute_b_with_positive_offset) {
  __arm_cpu cpu;
  arm_init_cpu(&cpu, NULL, NULL, NULL);
  cpu.r14 = 0xDEADBEEF;
  cpu.r15 = 0x24;
  __arm_instruction instr;
  instr.type = INSTR_BRANCH;
  instr.cond = COND_AL;
  instr.instr.branch.link = false;
  instr.instr.branch.offset = 8;
  
  arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r14, 0xDEADBEEF);
  cr_assert_eq(cpu.r15, 0x24+8);
}

Test(executor_branch, can_execute_b_with_negative_offset) {
  __arm_cpu cpu;
  arm_init_cpu(&cpu, NULL, NULL, NULL);
  cpu.r14 = 0xDEADBEEF;
  cpu.r15 = 0x24;
  __arm_instruction instr;
  instr.type = INSTR_BRANCH;
  instr.cond = COND_AL;
  instr.instr.branch.link = false;
  instr.instr.branch.offset = -8;
  
  arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r14, 0xDEADBEEF);
  cr_assert_eq(cpu.r15, 0x24-8);
}

Test(executor_branch, can_execute_bl_with_positive_offset) {
  __arm_cpu cpu;
  arm_init_cpu(&cpu, NULL, NULL, NULL);
  cpu.r14 = 0xDEADBEEF;
  cpu.r15 = 0x24;
  __arm_instruction instr;
  instr.type = INSTR_BRANCH;
  instr.cond = COND_AL;
  instr.instr.branch.link = true;
  instr.instr.branch.offset = 8;
  
  arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r14, 0x24);
  cr_assert_eq(cpu.r15, 0x24+8);
}


