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

  cpu.r3 = 1;
  cpu.r4 = 0x80000001;
  
  uint32_t result = eval_operand2(&cpu, &op, &carryValid, &carry);

  cr_assert_eq(result, 0x00000002);
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

Test(eval_operand2, can_evaluate_register_ror0_rrx_1_carry) {
  __arm_cpu cpu;
  arm_init_cpu(&cpu, NULL, NULL, NULL);
  bool carryValid;
  bool carry;
  SET_CARRY_FLAG(((__arm_cpu*)&cpu), 1);

  __arm_operand2 op;
  op.is_immediate = false;
  op.op.reg.reg = REG_R4;
  op.op.reg.is_register_shift = false;
  op.op.reg.shift_type = SHIFT_ROTATE_RIGHT;
  op.op.reg.shift_imm = 0;

  cpu.r4 = 0x7FFFFFFF;
  
  uint32_t result = eval_operand2(&cpu, &op, &carryValid, &carry);

  cr_assert_eq(result, 0xBFFFFFFF);
  cr_assert_eq(carryValid, true);
  cr_assert_eq(carry, true);
}

Test(eval_operand2, can_evaluate_register_ror0_rrx_0_carry) {
  __arm_cpu cpu;
  arm_init_cpu(&cpu, NULL, NULL, NULL);
  bool carryValid;
  bool carry;
  SET_CARRY_FLAG(((__arm_cpu*)&cpu), 0);

  __arm_operand2 op;
  op.is_immediate = false;
  op.op.reg.reg = REG_R4;
  op.op.reg.is_register_shift = false;
  op.op.reg.shift_type = SHIFT_ROTATE_RIGHT;
  op.op.reg.shift_imm = 0;

  cpu.r4 = 0x7FFFFFFE;
  
  uint32_t result = eval_operand2(&cpu, &op, &carryValid, &carry);

  cr_assert_eq(result, 0x3FFFFFFF);
  cr_assert_eq(carryValid, true);
  cr_assert_eq(carry, false);
}

Test(eval_operand2, can_evaluate_register_ror_r3_without_carry) {
  __arm_cpu cpu;
  arm_init_cpu(&cpu, NULL, NULL, NULL);
  bool carryValid;
  bool carry;

  __arm_operand2 op;
  op.is_immediate = false;
  op.op.reg.reg = REG_R4;
  op.op.reg.is_register_shift = true;
  op.op.reg.shift_type = SHIFT_ROTATE_RIGHT;
  op.op.reg.shift_reg = REG_R3;

  cpu.r3 = 4;
  cpu.r4 = 0x0FFFFFF4;
  
  uint32_t result = eval_operand2(&cpu, &op, &carryValid, &carry);

  cr_assert_eq(result, 0x40FFFFFF);
  cr_assert_eq(carryValid, true);
  cr_assert_eq(carry, false);
}

Test(eval_operand2, can_evaluate_register_ror_r3_with_carry) {
  __arm_cpu cpu;
  arm_init_cpu(&cpu, NULL, NULL, NULL);
  bool carryValid;
  bool carry;

  __arm_operand2 op;
  op.is_immediate = false;
  op.op.reg.reg = REG_R4;
  op.op.reg.is_register_shift = true;
  op.op.reg.shift_type = SHIFT_ROTATE_RIGHT;
  op.op.reg.shift_reg = REG_R3;

  cpu.r3 = 4;
  cpu.r4 = 0x0FFFFFF8;
  
  uint32_t result = eval_operand2(&cpu, &op, &carryValid, &carry);

  cr_assert_eq(result, 0x80FFFFFF);
  cr_assert_eq(carryValid, true);
  cr_assert_eq(carry, true);
}

Test(eval_operand2, can_evaluate_register_ror_r3_where_r3_is_0) {
  __arm_cpu cpu;
  arm_init_cpu(&cpu, NULL, NULL, NULL);
  bool carryValid;
  bool carry;

  __arm_operand2 op;
  op.is_immediate = false;
  op.op.reg.reg = REG_R4;
  op.op.reg.is_register_shift = true;
  op.op.reg.shift_type = SHIFT_ROTATE_RIGHT;
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

///////////////////////////////////////////
// AND
///////////////////////////////////////////

Test(executor_branch, can_execute_and_nc_nz_nn) {
  __arm_cpu cpu;
  arm_init_cpu(&cpu, NULL, NULL, NULL);
  cpu.r1 = 0x3;
  cpu.r2 = 0x1;
  __arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_AND;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  arm_execute_instruction(&cpu, &instr);

  __arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 1);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
}

Test(executor_branch, can_execute_and_nc_z_nn) {
  __arm_cpu cpu;
  arm_init_cpu(&cpu, NULL, NULL, NULL);
  cpu.r1 = 0x30;
  cpu.r2 = 0x1;
  __arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_AND;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  arm_execute_instruction(&cpu, &instr);

  __arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 0);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), true);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
}

Test(executor_branch, can_execute_and_nc_nz_n) {
  __arm_cpu cpu;
  arm_init_cpu(&cpu, NULL, NULL, NULL);
  cpu.r1 = 0xFFFFFFFF;
  cpu.r2 = 0x80000000;
  __arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_AND;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  arm_execute_instruction(&cpu, &instr);

  __arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 0x80000000);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), true);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
}

Test(executor_branch, can_execute_and_c_nz_nn) {
  __arm_cpu cpu;
  arm_init_cpu(&cpu, NULL, NULL, NULL);
  cpu.r1 = 0x7FFFFFFF;
  cpu.r2 = 0xFFFFFFFF;
  __arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_AND;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 1;
  instr.instr.data_processing.dest = REG_R0;
  
  arm_execute_instruction(&cpu, &instr);

  __arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 0x7FFFFFFE);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), true);
}

Test(executor_branch, can_execute_and_ns) {
  __arm_cpu cpu;
  arm_init_cpu(&cpu, NULL, NULL, NULL);
  cpu.r1 = 0x30;
  cpu.r2 = 0x1;
  __arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_AND;
  instr.instr.data_processing.set_condition_codes = false;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  arm_execute_instruction(&cpu, &instr);

  __arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 0);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
}

Test(executor_branch, can_execute_and_r15_dest) {
  __arm_cpu cpu;
  __arm_cpu* cpuptr = &cpu;
  arm_init_cpu(&cpu, NULL, NULL, NULL);
  arm_set_mode(&cpu, MODE_FIQ);
  cpu.r1 = 0x30;
  cpu.r2 = 0x1;
  cpu.cpsr |= 0x02345670;
  cpu.spsr_fiq = 0x06543210 | MODE_FIQ;
  __arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_AND;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R15;

  arm_execute_instruction(&cpu, &instr);
  
  cr_assert_eq(cpu.r0, 0);
  cr_assert_eq(cpu.cpsr, cpu.spsr_fiq);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
}

