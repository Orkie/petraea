#include <stdlib.h>
#include <stdio.h>
#include <criterion/criterion.h>
#include "cpu.h"
#include "instr.h"

///////////////////////////////////////////
// Evaluate operand 2
///////////////////////////////////////////

Test(_petraea_eval_operand2, can_evaluate_immediate_value) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  bool carryValid;
  bool carry;

  pt_arm_operand2 op;
  op.is_immediate = true;
  op.op.imm.value = 0x123456;
  op.op.imm.carryValid = true;
  op.op.imm.carry = false;
  
  uint32_t result = _petraea_eval_operand2(&cpu, &op, &carryValid, &carry);

  cr_assert_eq(result, 0x123456);
  cr_assert_eq(carryValid, true);
  cr_assert_eq(carry, false);
}

Test(_petraea_eval_operand2, can_evaluate_register_lsl0) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  bool carryValid;
  bool carry;

  pt_arm_operand2 op;
  op.is_immediate = false;
  op.op.reg.reg = REG_R4;
  op.op.reg.is_register_shift = false;
  op.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  op.op.reg.shift_imm = 0;

  cpu.r4 = 0x123456;
  
  uint32_t result = _petraea_eval_operand2(&cpu, &op, &carryValid, &carry);

  cr_assert_eq(result, 0x123456);
  cr_assert_eq(carryValid, false);
  cr_assert_eq(carry, false);
}

Test(_petraea_eval_operand2, can_evaluate_register_lsl_r3_no_carry) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  bool carryValid;
  bool carry;

  pt_arm_operand2 op;
  op.is_immediate = false;
  op.op.reg.reg = REG_R4;
  op.op.reg.is_register_shift = true;
  op.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  op.op.reg.shift_reg = REG_R3;

  cpu.r3 = 4;
  cpu.r4 = 0x123456;
  
  uint32_t result = _petraea_eval_operand2(&cpu, &op, &carryValid, &carry);

  cr_assert_eq(result, 0x1234560);
  cr_assert_eq(carryValid, true);
  cr_assert_eq(carry, false);
}

Test(_petraea_eval_operand2, can_evaluate_register_lsl_r3_with_carry) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  bool carryValid;
  bool carry;

  pt_arm_operand2 op;
  op.is_immediate = false;
  op.op.reg.reg = REG_R4;
  op.op.reg.is_register_shift = true;
  op.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  op.op.reg.shift_reg = REG_R3;

  cpu.r3 = 1;
  cpu.r4 = 0x80000001;
  
  uint32_t result = _petraea_eval_operand2(&cpu, &op, &carryValid, &carry);

  cr_assert_eq(result, 0x00000002);
  cr_assert_eq(carryValid, true);
  cr_assert_eq(carry, true);
}

Test(_petraea_eval_operand2, can_evaluate_register_lsl_r3_where_r3_is_0) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  bool carryValid;
  bool carry;

  pt_arm_operand2 op;
  op.is_immediate = false;
  op.op.reg.reg = REG_R4;
  op.op.reg.is_register_shift = true;
  op.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  op.op.reg.shift_reg = REG_R3;

  cpu.r3 = 0;
  cpu.r4 = 0x123456;
  
  uint32_t result = _petraea_eval_operand2(&cpu, &op, &carryValid, &carry);

  cr_assert_eq(result, 0x123456);
  cr_assert_eq(carryValid, true);
  cr_assert_eq(carry, false);
}

Test(_petraea_eval_operand2, can_evaluate_register_rsr0) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  bool carryValid;
  bool carry;

  pt_arm_operand2 op;
  op.is_immediate = false;
  op.op.reg.reg = REG_R4;
  op.op.reg.is_register_shift = false;
  op.op.reg.shift_type = SHIFT_LOGICAL_RIGHT;
  op.op.reg.shift_imm = 0;

  cpu.r4 = 0x80000000;
  
  uint32_t result = _petraea_eval_operand2(&cpu, &op, &carryValid, &carry);

  cr_assert_eq(result, 0x0);
  cr_assert_eq(carryValid, true);
  cr_assert_eq(carry, true);
}

Test(_petraea_eval_operand2, can_evaluate_register_rsr_r3_no_carry) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  bool carryValid;
  bool carry;

  pt_arm_operand2 op;
  op.is_immediate = false;
  op.op.reg.reg = REG_R4;
  op.op.reg.is_register_shift = true;
  op.op.reg.shift_type = SHIFT_LOGICAL_RIGHT;
  op.op.reg.shift_reg = REG_R3;

  cpu.r3 = 4;
  cpu.r4 = 0x1234560;
  
  uint32_t result = _petraea_eval_operand2(&cpu, &op, &carryValid, &carry);

  cr_assert_eq(result, 0x123456);
  cr_assert_eq(carryValid, true);
  cr_assert_eq(carry, false);
}

Test(_petraea_eval_operand2, can_evaluate_register_rsr_r3_with_carry) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  bool carryValid;
  bool carry;

  pt_arm_operand2 op;
  op.is_immediate = false;
  op.op.reg.reg = REG_R4;
  op.op.reg.is_register_shift = true;
  op.op.reg.shift_type = SHIFT_LOGICAL_RIGHT;
  op.op.reg.shift_reg = REG_R3;

  cpu.r3 = 4;
  cpu.r4 = 0xFFFFFFFF;
  
  uint32_t result = _petraea_eval_operand2(&cpu, &op, &carryValid, &carry);

  cr_assert_eq(result, 0x0FFFFFFF);
  cr_assert_eq(carryValid, true);
  cr_assert_eq(carry, true);
}

Test(_petraea_eval_operand2, can_evaluate_register_rsr_r3_where_r3_is_0) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  bool carryValid;
  bool carry;

  pt_arm_operand2 op;
  op.is_immediate = false;
  op.op.reg.reg = REG_R4;
  op.op.reg.is_register_shift = true;
  op.op.reg.shift_type = SHIFT_LOGICAL_RIGHT;
  op.op.reg.shift_reg = REG_R3;

  cpu.r3 = 0;
  cpu.r4 = 0x123456;
  
  uint32_t result = _petraea_eval_operand2(&cpu, &op, &carryValid, &carry);

  cr_assert_eq(result, 0x123456);
  cr_assert_eq(carryValid, true);
  cr_assert_eq(carry, false);
}

Test(_petraea_eval_operand2, can_evaluate_register_asr0_positive) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  bool carryValid;
  bool carry;

  pt_arm_operand2 op;
  op.is_immediate = false;
  op.op.reg.reg = REG_R4;
  op.op.reg.is_register_shift = false;
  op.op.reg.shift_type = SHIFT_ARITHMETIC_RIGHT;
  op.op.reg.shift_imm = 0;

  cpu.r4 = 0x80000000;
  
  uint32_t result = _petraea_eval_operand2(&cpu, &op, &carryValid, &carry);

  cr_assert_eq(result, 0xFFFFFFFF);
  cr_assert_eq(carryValid, true);
  cr_assert_eq(carry, true);
}

Test(_petraea_eval_operand2, can_evaluate_register_asr0_negative) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  bool carryValid;
  bool carry;

  pt_arm_operand2 op;
  op.is_immediate = false;
  op.op.reg.reg = REG_R4;
  op.op.reg.is_register_shift = false;
  op.op.reg.shift_type = SHIFT_ARITHMETIC_RIGHT;
  op.op.reg.shift_imm = 0;

  cpu.r4 = 0x00000000;
  
  uint32_t result = _petraea_eval_operand2(&cpu, &op, &carryValid, &carry);

  cr_assert_eq(result, 0x0);
  cr_assert_eq(carryValid, true);
  cr_assert_eq(carry, false);
}

Test(_petraea_eval_operand2, can_evaluate_register_asr_r3_no_carry_positive) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  bool carryValid;
  bool carry;

  pt_arm_operand2 op;
  op.is_immediate = false;
  op.op.reg.reg = REG_R4;
  op.op.reg.is_register_shift = true;
  op.op.reg.shift_type = SHIFT_ARITHMETIC_RIGHT;
  op.op.reg.shift_reg = REG_R3;

  cpu.r3 = 4;
  cpu.r4 = 0x123450;
  
  uint32_t result = _petraea_eval_operand2(&cpu, &op, &carryValid, &carry);

  cr_assert_eq(result, 0x12345);
  cr_assert_eq(carryValid, true);
  cr_assert_eq(carry, false);
}

Test(_petraea_eval_operand2, can_evaluate_register_asr_r3_no_carry_negative) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  bool carryValid;
  bool carry;

  pt_arm_operand2 op;
  op.is_immediate = false;
  op.op.reg.reg = REG_R4;
  op.op.reg.is_register_shift = true;
  op.op.reg.shift_type = SHIFT_ARITHMETIC_RIGHT;
  op.op.reg.shift_reg = REG_R3;

  cpu.r3 = 4;
  cpu.r4 = 0x80123450;
  
  uint32_t result = _petraea_eval_operand2(&cpu, &op, &carryValid, &carry);

  cr_assert_eq(result, 0xF8012345);
  cr_assert_eq(carryValid, true);
  cr_assert_eq(carry, false);
}

Test(_petraea_eval_operand2, can_evaluate_register_asr_r3_with_carry_positive) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  bool carryValid;
  bool carry;

  pt_arm_operand2 op;
  op.is_immediate = false;
  op.op.reg.reg = REG_R4;
  op.op.reg.is_register_shift = true;
  op.op.reg.shift_type = SHIFT_ARITHMETIC_RIGHT;
  op.op.reg.shift_reg = REG_R3;

  cpu.r3 = 4;
  cpu.r4 = 0x0FFFFFFF;
  
  uint32_t result = _petraea_eval_operand2(&cpu, &op, &carryValid, &carry);

  cr_assert_eq(result, 0x00FFFFFF);
  cr_assert_eq(carryValid, true);
  cr_assert_eq(carry, true);
}

Test(_petraea_eval_operand2, can_evaluate_register_asr_r3_with_carry_negative) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  bool carryValid;
  bool carry;

  pt_arm_operand2 op;
  op.is_immediate = false;
  op.op.reg.reg = REG_R4;
  op.op.reg.is_register_shift = true;
  op.op.reg.shift_type = SHIFT_ARITHMETIC_RIGHT;
  op.op.reg.shift_reg = REG_R3;

  cpu.r3 = 4;
  cpu.r4 = 0xFFFFFFFF;
  
  uint32_t result = _petraea_eval_operand2(&cpu, &op, &carryValid, &carry);

  cr_assert_eq(result, 0xFFFFFFFF);
  cr_assert_eq(carryValid, true);
  cr_assert_eq(carry, true);
}

Test(_petraea_eval_operand2, can_evaluate_register_asr_r3_where_r3_is_0) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  bool carryValid;
  bool carry;

  pt_arm_operand2 op;
  op.is_immediate = false;
  op.op.reg.reg = REG_R4;
  op.op.reg.is_register_shift = true;
  op.op.reg.shift_type = SHIFT_ARITHMETIC_RIGHT;
  op.op.reg.shift_reg = REG_R3;

  cpu.r3 = 0;
  cpu.r4 = 0x123456;
  
  uint32_t result = _petraea_eval_operand2(&cpu, &op, &carryValid, &carry);

  cr_assert_eq(result, 0x123456);
  cr_assert_eq(carryValid, true);
  cr_assert_eq(carry, false);
}

Test(_petraea_eval_operand2, can_evaluate_register_ror0_rrx_1_carry) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  bool carryValid;
  bool carry;
  SET_CARRY_FLAG(((pt_arm_cpu*)&cpu), 1);

  pt_arm_operand2 op;
  op.is_immediate = false;
  op.op.reg.reg = REG_R4;
  op.op.reg.is_register_shift = false;
  op.op.reg.shift_type = SHIFT_ROTATE_RIGHT;
  op.op.reg.shift_imm = 0;

  cpu.r4 = 0x7FFFFFFF;
  
  uint32_t result = _petraea_eval_operand2(&cpu, &op, &carryValid, &carry);

  cr_assert_eq(result, 0xBFFFFFFF);
  cr_assert_eq(carryValid, true);
  cr_assert_eq(carry, true);
}

Test(_petraea_eval_operand2, can_evaluate_register_ror0_rrx_0_carry) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  bool carryValid;
  bool carry;
  SET_CARRY_FLAG(((pt_arm_cpu*)&cpu), 0);

  pt_arm_operand2 op;
  op.is_immediate = false;
  op.op.reg.reg = REG_R4;
  op.op.reg.is_register_shift = false;
  op.op.reg.shift_type = SHIFT_ROTATE_RIGHT;
  op.op.reg.shift_imm = 0;

  cpu.r4 = 0x7FFFFFFE;
  
  uint32_t result = _petraea_eval_operand2(&cpu, &op, &carryValid, &carry);

  cr_assert_eq(result, 0x3FFFFFFF);
  cr_assert_eq(carryValid, true);
  cr_assert_eq(carry, false);
}

Test(_petraea_eval_operand2, can_evaluate_register_ror_r3_without_carry) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  bool carryValid;
  bool carry;

  pt_arm_operand2 op;
  op.is_immediate = false;
  op.op.reg.reg = REG_R4;
  op.op.reg.is_register_shift = true;
  op.op.reg.shift_type = SHIFT_ROTATE_RIGHT;
  op.op.reg.shift_reg = REG_R3;

  cpu.r3 = 4;
  cpu.r4 = 0x0FFFFFF4;
  
  uint32_t result = _petraea_eval_operand2(&cpu, &op, &carryValid, &carry);

  cr_assert_eq(result, 0x40FFFFFF);
  cr_assert_eq(carryValid, true);
  cr_assert_eq(carry, false);
}

Test(_petraea_eval_operand2, can_evaluate_register_ror_r3_with_carry) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  bool carryValid;
  bool carry;

  pt_arm_operand2 op;
  op.is_immediate = false;
  op.op.reg.reg = REG_R4;
  op.op.reg.is_register_shift = true;
  op.op.reg.shift_type = SHIFT_ROTATE_RIGHT;
  op.op.reg.shift_reg = REG_R3;

  cpu.r3 = 4;
  cpu.r4 = 0x0FFFFFF8;
  
  uint32_t result = _petraea_eval_operand2(&cpu, &op, &carryValid, &carry);

  cr_assert_eq(result, 0x80FFFFFF);
  cr_assert_eq(carryValid, true);
  cr_assert_eq(carry, true);
}

Test(_petraea_eval_operand2, can_evaluate_register_ror_r3_where_r3_is_0) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  bool carryValid;
  bool carry;

  pt_arm_operand2 op;
  op.is_immediate = false;
  op.op.reg.reg = REG_R4;
  op.op.reg.is_register_shift = true;
  op.op.reg.shift_type = SHIFT_ROTATE_RIGHT;
  op.op.reg.shift_reg = REG_R3;

  cpu.r3 = 0;
  cpu.r4 = 0x123456;
  
  uint32_t result = _petraea_eval_operand2(&cpu, &op, &carryValid, &carry);

  cr_assert_eq(result, 0x123456);
  cr_assert_eq(carryValid, true);
  cr_assert_eq(carry, false);
}

///////////////////////////////////////////
// Branch
///////////////////////////////////////////

Test(executor_branch, can_execute_b_with_positive_offset) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r14 = 0xDEADBEEF;
  cpu.r15 = 0x24;
  pt_arm_instruction instr;
  instr.type = INSTR_BRANCH;
  instr.cond = COND_AL;
  instr.instr.branch.link = false;
  instr.instr.branch.offset = 8;
  
  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r14, 0xDEADBEEF);
  cr_assert_eq(cpu.r15, 0x24+8);
}

Test(executor_branch, can_execute_b_with_negative_offset) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r14 = 0xDEADBEEF;
  cpu.r15 = 0x24;
  pt_arm_instruction instr;
  instr.type = INSTR_BRANCH;
  instr.cond = COND_AL;
  instr.instr.branch.link = false;
  instr.instr.branch.offset = -8;
  
  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r14, 0xDEADBEEF);
  cr_assert_eq(cpu.r15, 0x24-8);
}

Test(executor_branch, can_execute_bl_with_positive_offset) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r14 = 0xDEADBEEF;
  cpu.r15 = 0x24;
  pt_arm_instruction instr;
  instr.type = INSTR_BRANCH;
  instr.cond = COND_AL;
  instr.instr.branch.link = true;
  instr.instr.branch.offset = 8;
  
  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r14, 0x20);
  cr_assert_eq(cpu.r15, 0x24+8);
}

///////////////////////////////////////////
// Branch
///////////////////////////////////////////

Test(executor_branch_and_exchange, can_execute_bx_to_arm) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r0 = 0x12345678;
  pt_arm_instruction instr;
  instr.type = INSTR_BRANCH_EXCHANGE;
  instr.cond = COND_AL;
  instr.instr.branch_exchange.operand = REG_R0;

  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r15, 0x12345678);
  cr_assert_eq(GET_THUMB_FLAG(((pt_arm_cpu*)&cpu)), false); 
}

Test(executor_branch_and_exchange, can_execute_bx_to_thumb) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r0 = 0x12345679;
  pt_arm_instruction instr;
  instr.type = INSTR_BRANCH_EXCHANGE;
  instr.cond = COND_AL;
  instr.instr.branch_exchange.operand = REG_R0;

  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r15, 0x12345678);
  cr_assert_eq(GET_THUMB_FLAG(((pt_arm_cpu*)&cpu)), true); 
}

///////////////////////////////////////////
// AND
///////////////////////////////////////////

Test(executor_and, can_execute_and_nc_nz_nn) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r1 = 0x3;
  cpu.r2 = 0x1;
  pt_arm_instruction instr;
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
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 1);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
}

Test(executor_and, can_execute_and_nc_z_nn) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r1 = 0x30;
  cpu.r2 = 0x1;
  pt_arm_instruction instr;
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
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 0);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), true);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
}

Test(executor_and, can_execute_and_nc_nz_n) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r1 = 0xFFFFFFFF;
  cpu.r2 = 0x80000000;
  pt_arm_instruction instr;
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
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 0x80000000);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), true);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
}

Test(executor_and, can_execute_and_c_nz_nn) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r1 = 0x7FFFFFFF;
  cpu.r2 = 0xFFFFFFFF;
  pt_arm_instruction instr;
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
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 0x7FFFFFFE);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), true);
}

Test(executor_and, can_execute_and_ns) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r1 = 0x30;
  cpu.r2 = 0x1;
  pt_arm_instruction instr;
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
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 0);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
}

Test(executor_and, can_execute_and_r15_dest) {
  pt_arm_cpu cpu;
  pt_arm_cpu* cpuptr = &cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  pt_arm_set_mode(&cpu, MODE_FIQ);
  cpu.r1 = 0x30;
  cpu.r2 = 0x1;
  cpu.cpsr |= 0x02345670;
  cpu.spsr_fiq = 0x06543210 | MODE_FIQ;
  pt_arm_instruction instr;
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

  pt_arm_execute_instruction(&cpu, &instr);
  
  cr_assert_eq(cpu.r15, 0);
  cr_assert_eq(cpu.cpsr, cpu.spsr_fiq);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
}

///////////////////////////////////////////
// EOR
///////////////////////////////////////////

Test(executor_eor, can_execute_eor_nc_nz_nn) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r1 = 0x55555555;
  cpu.r2 = 0x5555555A;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_EOR;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 0xF);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
}

Test(executor_eor, can_execute_eor_nc_z_nn) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r1 = 0x55555555;
  cpu.r2 = 0x55555555;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_EOR;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 0);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), true);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
}

Test(executor_eor, can_execute_eor_nc_nz_n) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r1 = 0x55555555;
  cpu.r2 = 0xAAAAAAAA;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_EOR;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 0xFFFFFFFF);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), true);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
}

Test(executor_eor, can_execute_eor_c_nz_nn) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r1 = 0xFFFFFFFF;
  cpu.r2 = 0xFFFFFFFF;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_EOR;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 1;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 0x1);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), true);
}

Test(executor_eor, can_execute_eor_r15_dest) {
  pt_arm_cpu cpu;
  pt_arm_cpu* cpuptr = &cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  pt_arm_set_mode(&cpu, MODE_FIQ);
  cpu.r1 = 0xAAAAAAAA;
  cpu.r2 = 0x55555555;
  cpu.cpsr |= 0x02345670;
  cpu.spsr_fiq = 0x06543210 | MODE_FIQ;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_EOR;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R15;

  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r15, 0xFFFFFFFF);
  cr_assert_eq(cpu.cpsr, cpu.spsr_fiq);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
}

///////////////////////////////////////////
// SUB
///////////////////////////////////////////

Test(executor_sub, can_execute_sub_nc_nz_n_nv) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r1 = 3;
  cpu.r2 = 8;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_SUB;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, -5);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), true);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
  cr_assert_eq(GET_OVERFLOW_FLAG(cpuptr), false);
}

Test(executor_sub, can_execute_sub_c_nz_nn_nv) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r1 = 8;
  cpu.r2 = 3;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_SUB;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 5);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), true);
  cr_assert_eq(GET_OVERFLOW_FLAG(cpuptr), false);
}

Test(executor_sub, can_execute_sub_c_z_nn_v) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r1 = 8;
  cpu.r2 = 8;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_SUB;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 0);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), true);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
  cr_assert_eq(GET_OVERFLOW_FLAG(cpuptr), false);
}

Test(executor_sub, can_execute_sub_c_nz_nn_v) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r1 = 0x80000000;
  cpu.r2 = 1;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_SUB;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 0x7FFFFFFF);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), true);
  cr_assert_eq(GET_OVERFLOW_FLAG(cpuptr), true);
}

///////////////////////////////////////////
// RSB
///////////////////////////////////////////

Test(executor_rsb, can_execute_rsb_nc_nz_n_nv) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r1 = 8;
  cpu.r2 = 3;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_RSB;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, -5);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), true);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
  cr_assert_eq(GET_OVERFLOW_FLAG(cpuptr), false);
}

Test(executor_rsb, can_execute_rsb_c_nz_nn_nv) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r1 = 3;
  cpu.r2 = 8;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_RSB;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 5);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), true);
  cr_assert_eq(GET_OVERFLOW_FLAG(cpuptr), false);
}

Test(executor_rsb, can_execute_rsb_c_z_nn_v) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r1 = 8;
  cpu.r2 = 8;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_RSB;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 0);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), true);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
  cr_assert_eq(GET_OVERFLOW_FLAG(cpuptr), false);
}

Test(executor_rsb, can_execute_rsb_c_nz_nn_v) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r1 = 1;
  cpu.r2 = 0x80000000;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_RSB;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 0x7FFFFFFF);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), true);
  cr_assert_eq(GET_OVERFLOW_FLAG(cpuptr), true);
}

///////////////////////////////////////////
// ADD
///////////////////////////////////////////

Test(executor_add, can_execute_add_nc_nz_nn_nv) {
  pt_arm_cpu cpu;
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
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 11);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
  cr_assert_eq(GET_OVERFLOW_FLAG(cpuptr), false);
}

Test(executor_add, can_execute_add_nc_nz_n_nv) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r1 = 8;
  cpu.r2 = -12;
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
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, -4);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), true);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
  cr_assert_eq(GET_OVERFLOW_FLAG(cpuptr), false);
}

Test(executor_add, can_execute_add_nc_nz_n_v) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r1 = 0x7FFFFFFF;
  cpu.r2 = 1;
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
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 0x80000000);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), true);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
  cr_assert_eq(GET_OVERFLOW_FLAG(cpuptr), true);
}

Test(executor_add, can_execute_add_c_nz_nn_nv) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r1 = 0xFFFFFFFF;
  cpu.r2 = 2;
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
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 1);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), true);
  cr_assert_eq(GET_OVERFLOW_FLAG(cpuptr), false);
}

Test(executor_add, can_execute_add_nc_z_nn_nv) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r1 = 0;
  cpu.r2 = 0;
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
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 0);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), true);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
  cr_assert_eq(GET_OVERFLOW_FLAG(cpuptr), false);
}

///////////////////////////////////////////
// ADC
///////////////////////////////////////////

Test(executor_adc, can_execute_adc_nc_nz_nn_nv_carry_not_set) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  pt_arm_cpu* cpuptr = &cpu;
  SET_CARRY_FLAG(cpuptr, false);
  cpu.r1 = 8;
  cpu.r2 = 3;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_ADC;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r0, 11);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
  cr_assert_eq(GET_OVERFLOW_FLAG(cpuptr), false);
}

Test(executor_adc, can_execute_adc_nc_nz_n_nv_carry_not_set) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  pt_arm_cpu* cpuptr = &cpu;
  SET_CARRY_FLAG(cpuptr, false);
  cpu.r1 = 8;
  cpu.r2 = -12;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_ADC;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r0, -4);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), true);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
  cr_assert_eq(GET_OVERFLOW_FLAG(cpuptr), false);
}

Test(executor_adc, can_execute_adc_nc_nz_n_v_carry_not_set) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  pt_arm_cpu* cpuptr = &cpu;
  SET_CARRY_FLAG(cpuptr, false);
  cpu.r1 = 0x7FFFFFFF;
  cpu.r2 = 1;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_ADC;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r0, 0x80000000);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), true);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
  cr_assert_eq(GET_OVERFLOW_FLAG(cpuptr), true);
}

Test(executor_adc, can_execute_adc_c_nz_nn_nv_carry_not_set) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  pt_arm_cpu* cpuptr = &cpu;
  SET_CARRY_FLAG(cpuptr, false);
  cpu.r1 = 0xFFFFFFFF;
  cpu.r2 = 2;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_ADC;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r0, 1);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), true);
  cr_assert_eq(GET_OVERFLOW_FLAG(cpuptr), false);
}

Test(executor_adc, can_execute_adc_nc_z_nn_nv_carry_not_set) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  pt_arm_cpu* cpuptr = &cpu;
  SET_CARRY_FLAG(cpuptr, false);
  cpu.r1 = 0;
  cpu.r2 = 0;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_ADC;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r0, 0);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), true);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
  cr_assert_eq(GET_OVERFLOW_FLAG(cpuptr), false);
}

Test(executor_adc, can_execute_adc_nc_nz_nn_nv_carry_set) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  pt_arm_cpu* cpuptr = &cpu;
  SET_CARRY_FLAG(cpuptr, true);
  cpu.r1 = 8;
  cpu.r2 = 3;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_ADC;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r0, 12);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
  cr_assert_eq(GET_OVERFLOW_FLAG(cpuptr), false);
}

Test(executor_adc, can_execute_adc_nc_nz_n_nv_carry_set) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  pt_arm_cpu* cpuptr = &cpu;
  SET_CARRY_FLAG(cpuptr, true);
  cpu.r1 = -3;
  cpu.r2 = 1;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_ADC;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);
  
  cr_assert_eq(cpu.r0, -1);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), true);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
  cr_assert_eq(GET_OVERFLOW_FLAG(cpuptr), false);
}

Test(executor_adc, can_execute_adc_nc_nz_n_v_carry_set) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  pt_arm_cpu* cpuptr = &cpu;
  SET_CARRY_FLAG(cpuptr, true);
  cpu.r1 = 0x7FFFFFFE;
  cpu.r2 = 1;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_ADC;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r0, 0x80000000);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), true);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
  cr_assert_eq(GET_OVERFLOW_FLAG(cpuptr), true);
}

Test(executor_adc, can_execute_adc_c_nz_nn_nv_carry_set) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  pt_arm_cpu* cpuptr = &cpu;
  SET_CARRY_FLAG(cpuptr, true);
  cpu.r1 = 0xFFFFFFFF;
  cpu.r2 = 1;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_ADC;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r0, 1);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), true);
  cr_assert_eq(GET_OVERFLOW_FLAG(cpuptr), false);
}

Test(executor_adc, can_execute_adc_c_z_nn_nv_carry_set) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  pt_arm_cpu* cpuptr = &cpu;
  SET_CARRY_FLAG(cpuptr, true);
  cpu.r1 = -1;
  cpu.r2 = 0;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_ADC;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r0, 0);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), true);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), true);
  cr_assert_eq(GET_OVERFLOW_FLAG(cpuptr), false);
}

///////////////////////////////////////////
// SBC
///////////////////////////////////////////

Test(executor_sbc, can_execute_sbc_nc_nz_n_nv_carry_set) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  pt_arm_cpu* cpuptr = &cpu;
  SET_CARRY_FLAG(cpuptr, true);
  cpu.r1 = 3;
  cpu.r2 = 8;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_SBC;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r0, -5);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), true);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
  cr_assert_eq(GET_OVERFLOW_FLAG(cpuptr), false);
}

Test(executor_sbc, can_execute_sbc_c_nz_nn_nv_carry_set) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  pt_arm_cpu* cpuptr = &cpu;
  SET_CARRY_FLAG(cpuptr, true);
  cpu.r1 = 8;
  cpu.r2 = 3;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_SBC;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r0, 5);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), true);
  cr_assert_eq(GET_OVERFLOW_FLAG(cpuptr), false);
}

Test(executor_sbc, can_execute_sbc_c_z_nn_v_carry_set) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  pt_arm_cpu* cpuptr = &cpu;
  SET_CARRY_FLAG(cpuptr, true);
  cpu.r1 = 8;
  cpu.r2 = 8;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_SBC;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r0, 0);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), true);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
  cr_assert_eq(GET_OVERFLOW_FLAG(cpuptr), false);
}

Test(executor_sbc, can_execute_sbc_c_nz_nn_v_carry_set) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  pt_arm_cpu* cpuptr = &cpu;
  SET_CARRY_FLAG(cpuptr, true);
  cpu.r1 = 0x80000000;
  cpu.r2 = 1;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_SBC;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r0, 0x7FFFFFFF);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), true);
  cr_assert_eq(GET_OVERFLOW_FLAG(cpuptr), true);
}

Test(executor_sbc, can_execute_sbc_nc_nz_n_nv_carry_not_set) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  pt_arm_cpu* cpuptr = &cpu;
  SET_CARRY_FLAG(cpuptr, false);
  cpu.r1 = 3;
  cpu.r2 = 7;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_SBC;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r0, -5);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), true);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
  cr_assert_eq(GET_OVERFLOW_FLAG(cpuptr), false);
}

Test(executor_sbc, can_execute_sbc_c_nz_nn_nv_carry_not_set) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  pt_arm_cpu* cpuptr = &cpu;
  SET_CARRY_FLAG(cpuptr, false);
  cpu.r1 = 8;
  cpu.r2 = 2;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_SBC;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r0, 5);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), true);
  cr_assert_eq(GET_OVERFLOW_FLAG(cpuptr), false);
}

Test(executor_sbc, can_execute_sbc_nc_z_nn_v_carry_not_set) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  pt_arm_cpu* cpuptr = &cpu;
  SET_CARRY_FLAG(cpuptr, false);
  cpu.r1 = 8;
  cpu.r2 = 7;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_SBC;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r0, 0);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), true);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
  cr_assert_eq(GET_OVERFLOW_FLAG(cpuptr), false);
}

Test(executor_sbc, can_execute_sbc_c_nz_nn_v_carry_not_set) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  pt_arm_cpu* cpuptr = &cpu;
  SET_CARRY_FLAG(cpuptr, false);
  cpu.r1 = 0x80000000;
  cpu.r2 = 0;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_SBC;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r0, 0x7FFFFFFF);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), true);
  cr_assert_eq(GET_OVERFLOW_FLAG(cpuptr), true);
}

///////////////////////////////////////////
// RSC
///////////////////////////////////////////

Test(executor_rsc, can_execute_rsc_nc_nz_n_nv_carry_set) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  pt_arm_cpu* cpuptr = &cpu;
  SET_CARRY_FLAG(cpuptr, true);
  cpu.r1 = 8;
  cpu.r2 = 3;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_RSC;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r0, -5);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), true);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
  cr_assert_eq(GET_OVERFLOW_FLAG(cpuptr), false);
}

Test(executor_rsc, can_execute_rsc_c_nz_nn_nv_carry_set) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  pt_arm_cpu* cpuptr = &cpu;
  SET_CARRY_FLAG(cpuptr, true);
  cpu.r1 = 3;
  cpu.r2 = 8;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_RSC;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r0, 5);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), true);
  cr_assert_eq(GET_OVERFLOW_FLAG(cpuptr), false);
}

Test(executor_rsc, can_execute_rsc_c_z_nn_v_carry_set) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  pt_arm_cpu* cpuptr = &cpu;
  SET_CARRY_FLAG(cpuptr, true);
  cpu.r1 = 8;
  cpu.r2 = 8;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_RSC;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r0, 0);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), true);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
  cr_assert_eq(GET_OVERFLOW_FLAG(cpuptr), false);
}

Test(executor_rsc, can_execute_rsc_c_nz_nn_v_carry_set) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  pt_arm_cpu* cpuptr = &cpu;
  SET_CARRY_FLAG(cpuptr, true);
  cpu.r1 = 1;
  cpu.r2 = 0x80000000;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_RSC;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r0, 0x7FFFFFFF);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), true);
  cr_assert_eq(GET_OVERFLOW_FLAG(cpuptr), true);
}

Test(executor_rsc, can_execute_rsc_nc_nz_n_nv_carry_not_set) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  pt_arm_cpu* cpuptr = &cpu;
  SET_CARRY_FLAG(cpuptr, false);
  cpu.r1 = 7;
  cpu.r2 = 3;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_RSC;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r0, -5);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), true);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
  cr_assert_eq(GET_OVERFLOW_FLAG(cpuptr), false);
}

Test(executor_rsc, can_execute_rsc_c_nz_nn_nv_carry_not_set) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  pt_arm_cpu* cpuptr = &cpu;
  SET_CARRY_FLAG(cpuptr, false);
  cpu.r1 = 2;
  cpu.r2 = 8;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_RSC;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r0, 5);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), true);
  cr_assert_eq(GET_OVERFLOW_FLAG(cpuptr), false);
}

Test(executor_rsc, can_execute_rsc_nc_z_nn_v_carry_not_set) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  pt_arm_cpu* cpuptr = &cpu;
  SET_CARRY_FLAG(cpuptr, false);
  cpu.r1 = 7;
  cpu.r2 = 8;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_RSC;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r0, 0);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), true);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
  cr_assert_eq(GET_OVERFLOW_FLAG(cpuptr), false);
}

Test(executor_rsc, can_execute_rsc_c_nz_nn_v_carry_not_set) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  pt_arm_cpu* cpuptr = &cpu;
  SET_CARRY_FLAG(cpuptr, false);
  cpu.r1 = 0;
  cpu.r2 = 0x80000000;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_RSC;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r0, 0x7FFFFFFF);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), true);
  cr_assert_eq(GET_OVERFLOW_FLAG(cpuptr), true);
}

///////////////////////////////////////////
// TST
///////////////////////////////////////////

Test(executor_tst, can_execute_tst_nc_nz_nn) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r0 = 0xDEADBEEF;
  cpu.r1 = 0x3;
  cpu.r2 = 0x1;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_TST;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 0xDEADBEEF);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
}

Test(executor_tst, can_execute_tst_nc_z_nn) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r0 = 0xDEADBEEF;
  cpu.r1 = 0x30;
  cpu.r2 = 0x1;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_TST;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 0xDEADBEEF);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), true);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
}

Test(executor_tst, can_execute_tst_nc_nz_n) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r0 = 0xDEADBEEF;
  cpu.r1 = 0xFFFFFFFF;
  cpu.r2 = 0x80000000;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_TST;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 0xDEADBEEF);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), true);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
}

Test(executor_tst, can_execute_tst_c_nz_nn) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r0 = 0xDEADBEEF;
  cpu.r1 = 0x7FFFFFFF;
  cpu.r2 = 0xFFFFFFFF;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_TST;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 1;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 0xDEADBEEF);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), true);
}

Test(executor_tst, can_execute_tst_ns) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r0 = 0xDEADBEEF;
  cpu.r1 = 0x30;
  cpu.r2 = 0x1;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_TST;
  instr.instr.data_processing.set_condition_codes = false;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 0xDEADBEEF);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), true);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
}

///////////////////////////////////////////
// TEQ
///////////////////////////////////////////

Test(executor_teq, can_execute_teq_nc_nz_nn) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r0 = 0xDEADBEEF;
  cpu.r1 = 0x55555555;
  cpu.r2 = 0x5555555A;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_TEQ;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 0xDEADBEEF);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
}

Test(executor_teq, can_execute_teq_nc_z_nn) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r0 = 0xDEADBEEF;
  cpu.r1 = 0x55555555;
  cpu.r2 = 0x55555555;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_TEQ;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 0xDEADBEEF);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), true);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
}

Test(executor_teq, can_execute_teq_nc_nz_n) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r0 = 0xDEADBEEF;
  cpu.r1 = 0x55555555;
  cpu.r2 = 0xF5555555;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_TEQ;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 0xDEADBEEF);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), true);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
}

Test(executor_teq, can_execute_teq_c_nz_nn) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r0 = 0xDEADBEEF;
  cpu.r1 = 0x55555555;
  cpu.r2 = 0xFFFFFFFF;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_TEQ;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 1;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 0xDEADBEEF);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), true);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), true);
}

///////////////////////////////////////////
// CMP
///////////////////////////////////////////

Test(executor_cmp, can_execute_cmp_nc_nz_n_nv) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r0 = 0xDEADBEEF;
  cpu.r1 = 3;
  cpu.r2 = 8;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_CMP;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 0xDEADBEEF);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), true);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
  cr_assert_eq(GET_OVERFLOW_FLAG(cpuptr), false);
}

Test(executor_cmp, can_execute_cmp_c_nz_nn_nv) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r0 = 0xDEADBEEF;
  cpu.r1 = 8;
  cpu.r2 = 3;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_CMP;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 0xDEADBEEF);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), true);
  cr_assert_eq(GET_OVERFLOW_FLAG(cpuptr), false);
}

Test(executor_cmp, can_execute_cmp_c_z_nn_v) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r0 = 0xDEADBEEF;
  cpu.r1 = 8;
  cpu.r2 = 8;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_CMP;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 0xDEADBEEF);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), true);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
  cr_assert_eq(GET_OVERFLOW_FLAG(cpuptr), false);
}

Test(executor_cmp, can_execute_cmp_c_nz_nn_v) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r0 = 0xDEADBEEF;
  cpu.r1 = 0x80000000;
  cpu.r2 = 1;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_CMP;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 0xDEADBEEF);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), true);
  cr_assert_eq(GET_OVERFLOW_FLAG(cpuptr), true);
}

///////////////////////////////////////////
// CMN
///////////////////////////////////////////

Test(executor_cmn, can_execute_cmn_nc_nz_nn_nv) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r0 = 0xDEADBEEF;
  cpu.r1 = 8;
  cpu.r2 = 3;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_CMN;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 0xDEADBEEF);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
  cr_assert_eq(GET_OVERFLOW_FLAG(cpuptr), false);
}

Test(executor_cmn, can_execute_cmn_nc_nz_n_nv) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r0 = 0xDEADBEEF;
  cpu.r1 = 8;
  cpu.r2 = -12;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_CMN;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 0xDEADBEEF);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), true);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
  cr_assert_eq(GET_OVERFLOW_FLAG(cpuptr), false);
}

Test(executor_cmn, can_execute_cmn_nc_nz_n_v) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r0 = 0xDEADBEEF;
  cpu.r1 = 0x7FFFFFFF;
  cpu.r2 = 1;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_CMN;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 0xDEADBEEF);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), true);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
  cr_assert_eq(GET_OVERFLOW_FLAG(cpuptr), true);
}

Test(executor_cmn, can_execute_cmn_c_nz_nn_nv) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r0 = 0xDEADBEEF;
  cpu.r1 = 0xFFFFFFFF;
  cpu.r2 = 2;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_CMN;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 0xDEADBEEF);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), true);
  cr_assert_eq(GET_OVERFLOW_FLAG(cpuptr), false);
}

Test(executor_cmn, can_execute_cmn_nc_z_nn_nv) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r0 = 0xDEADBEEF;
  cpu.r1 = 0;
  cpu.r2 = 0;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_CMN;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 0xDEADBEEF);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), true);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
  cr_assert_eq(GET_OVERFLOW_FLAG(cpuptr), false);
}

///////////////////////////////////////////
// ORR
///////////////////////////////////////////

Test(executor_orr, can_execute_orr_nc_nz_nn) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r1 = 0x0;
  cpu.r2 = 0x1;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_ORR;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 0x1);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
}

Test(executor_orr, can_execute_orr_nc_z_nn) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r1 = 0x0;
  cpu.r2 = 0x0;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_ORR;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 0x0);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), true);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
}

Test(executor_orr, can_execute_orr_nc_nz_n) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r1 = 0x80000000;
  cpu.r2 = 0x55555555;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_ORR;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 0xD5555555);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), true);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
}

Test(executor_orr, can_execute_orr_c_nz_n) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r1 = 0x3FFFFFFE;
  cpu.r2 = 0xFFFFFFFF;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_ORR;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 1;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 0xFFFFFFFE);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), true);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), true);
}

Test(executor_orr, can_execute_orr_r15_dest) {
  pt_arm_cpu cpu;
  pt_arm_cpu* cpuptr = &cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  pt_arm_set_mode(&cpu, MODE_FIQ);
  cpu.r1 = 0xAAAAAAAA;
  cpu.r2 = 0x55555555;
  cpu.cpsr |= 0x02345670;
  cpu.spsr_fiq = 0x06543210 | MODE_FIQ;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_ORR;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R15;

  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r15, 0xFFFFFFFF);
  cr_assert_eq(cpu.cpsr, cpu.spsr_fiq);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
}

///////////////////////////////////////////
// MOV
///////////////////////////////////////////

Test(executor_mov, can_execute_mov_nc_nz_nn) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r2 = 3;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_MOV;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 3);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
}

Test(executor_mov, can_execute_mov_nc_nz_n) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r2 = -12;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_MOV;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, -12);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), true);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
}

Test(executor_mov, can_execute_mov_nc_z_nn) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r0 = 0xDEADBEEF;
  cpu.r2 = 0;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_MOV;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 0x0);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), true);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
}

Test(executor_mov, can_execute_mov_c_nz_nn) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r2 = 0x8FFFFFFF;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_MOV;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 1;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 0x1FFFFFFE);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), true);
}

///////////////////////////////////////////
// BIC
///////////////////////////////////////////

Test(executor_bic, can_execute_bic_nc_nz_nn) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r1 = 0x3;
  cpu.r2 = 0xFFFFFFFE;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_BIC;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 1);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
}

Test(executor_bic, can_execute_bic_nc_z_nn) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r1 = 0x30;
  cpu.r2 = 0xFFFFFFFE;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_BIC;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 0);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), true);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
}

Test(executor_bic, can_execute_bic_nc_nz_n) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r1 = 0xFFFFFFFF;
  cpu.r2 = 0x7FFFFFFF;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_BIC;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 0x80000000);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), true);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
}

Test(executor_bic, can_execute_bic_c_nz_nn) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r1 = 0x7FFFFFFF;
  cpu.r2 = 0x80000001;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_BIC;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 1;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 0x7FFFFFFD);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), true);
}

Test(executor_bic, can_execute_bic_ns) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r1 = 0x30;
  cpu.r2 = 0xFFFFFFFE;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_BIC;
  instr.instr.data_processing.set_condition_codes = false;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 0);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
}

Test(executor_bic, can_execute_bic_r15_dest) {
  pt_arm_cpu cpu;
  pt_arm_cpu* cpuptr = &cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  pt_arm_set_mode(&cpu, MODE_FIQ);
  cpu.r1 = 0x30;
  cpu.r2 = 0xFFFFFFFE;
  cpu.cpsr |= 0x02345670;
  cpu.spsr_fiq = 0x06543210 | MODE_FIQ;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_BIC;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R15;

  pt_arm_execute_instruction(&cpu, &instr);
  
  cr_assert_eq(cpu.r15, 0);
  cr_assert_eq(cpu.cpsr, cpu.spsr_fiq);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
}

///////////////////////////////////////////
// MVN
///////////////////////////////////////////

Test(executor_mvn, can_execute_mvn_nc_nz_nn) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r2 = 0xFFFFFFFC;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_MVN;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 3);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
}

Test(executor_mvn, can_execute_mvn_nc_nz_n) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r2 = 0xB;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_MVN;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, -12);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), true);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
}

Test(executor_mvn, can_execute_mvn_nc_z_nn) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r0 = 0xDEADBEEF;
  cpu.r2 = 0xFFFFFFFF;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_MVN;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 0;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 0x0);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), true);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), false);
}

Test(executor_mvn, can_execute_mvn_c_nz_nn) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r2 = 0xE0000000;
  pt_arm_instruction instr;
  instr.type = INSTR_DATA_PROCESSING;
  instr.cond = COND_AL;
  instr.instr.data_processing.opcode = OPCODE_MVN;
  instr.instr.data_processing.set_condition_codes = true;
  instr.instr.data_processing.operand1 = REG_R1;
  instr.instr.data_processing.operand2.is_immediate = false;
  instr.instr.data_processing.operand2.op.reg.reg = REG_R2;
  instr.instr.data_processing.operand2.op.reg.is_register_shift = false;
  instr.instr.data_processing.operand2.op.reg.shift_type = SHIFT_LOGICAL_LEFT;
  instr.instr.data_processing.operand2.op.reg.shift_imm = 1;
  instr.instr.data_processing.dest = REG_R0;
  
  pt_arm_execute_instruction(&cpu, &instr);

  pt_arm_cpu* cpuptr = &cpu;
  cr_assert_eq(cpu.r0, 0x3FFFFFFF);
  cr_assert_eq(GET_NEGATIVE_FLAG(cpuptr), false);
  cr_assert_eq(GET_ZERO_FLAG(cpuptr), false);
  cr_assert_eq(GET_CARRY_FLAG(cpuptr), true);
}

///////////////////////////////////////////
// Conditions
///////////////////////////////////////////

#define SET_N (1<<31)
#define SET_Z (1<<30)
#define SET_C (1<<29)
#define SET_V (1<<28)

Test(executor_conditions, EQ_nz) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  
  bool result = _petraea_eval_condition(&cpu, COND_EQ);
  cr_assert_eq(result, false);
}

Test(executor_conditions, EQ_z) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.cpsr = SET_Z;
  
  bool result = _petraea_eval_condition(&cpu, COND_EQ);
  cr_assert_eq(result, true);
}

Test(executor_conditions, NE_nz) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  
  bool result = _petraea_eval_condition(&cpu, COND_NE);
  cr_assert_eq(result, true);
}

Test(executor_conditions, NE_z) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.cpsr = SET_Z;
  
  bool result = _petraea_eval_condition(&cpu, COND_NE);
  cr_assert_eq(result, false);
}

Test(executor_conditions, CS_nc) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  
  bool result = _petraea_eval_condition(&cpu, COND_CS);
  cr_assert_eq(result, false);
}

Test(executor_conditions, CS_c) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.cpsr = SET_C;
  
  bool result = _petraea_eval_condition(&cpu, COND_CS);
  cr_assert_eq(result, true);
}

Test(executor_conditions, CC_nc) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  
  bool result = _petraea_eval_condition(&cpu, COND_CC);
  cr_assert_eq(result, true);
}

Test(executor_conditions, CC_c) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.cpsr = SET_C;
  
  bool result = _petraea_eval_condition(&cpu, COND_CC);
  cr_assert_eq(result, false);
}

Test(executor_conditions, MI_nn) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  
  bool result = _petraea_eval_condition(&cpu, COND_MI);
  cr_assert_eq(result, false);
}

Test(executor_conditions, MI_n) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.cpsr = SET_N;
  
  bool result = _petraea_eval_condition(&cpu, COND_MI);
  cr_assert_eq(result, true);
}

Test(executor_conditions, PL_nn) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  
  bool result = _petraea_eval_condition(&cpu, COND_PL);
  cr_assert_eq(result, true);
}

Test(executor_conditions, PL_n) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.cpsr = SET_N;
  
  bool result = _petraea_eval_condition(&cpu, COND_PL);
  cr_assert_eq(result, false);
}

Test(executor_conditions, VS_nv) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  
  bool result = _petraea_eval_condition(&cpu, COND_VS);
  cr_assert_eq(result, false);
}

Test(executor_conditions, VS_v) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.cpsr = SET_V;
  
  bool result = _petraea_eval_condition(&cpu, COND_VS);
  cr_assert_eq(result, true);
}

Test(executor_conditions, VC_nv) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  
  bool result = _petraea_eval_condition(&cpu, COND_VC);
  cr_assert_eq(result, true);
}

Test(executor_conditions, VC_v) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.cpsr = SET_V;
  
  bool result = _petraea_eval_condition(&cpu, COND_VC);
  cr_assert_eq(result, false);
}

Test(executor_conditions, HI_c_nz) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.cpsr = SET_C;
  
  bool result = _petraea_eval_condition(&cpu, COND_HI);
  cr_assert_eq(result, true);
}

Test(executor_conditions, HI_c_z) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL); 
  cpu.cpsr = SET_C | SET_Z;
 
  bool result = _petraea_eval_condition(&cpu, COND_HI);
  cr_assert_eq(result, false);
}

Test(executor_conditions, HI_nc_nz) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL); 
 
  bool result = _petraea_eval_condition(&cpu, COND_HI);
  cr_assert_eq(result, false);
}

Test(executor_conditions, LS_nc_nz) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL); 
 
  bool result = _petraea_eval_condition(&cpu, COND_LS);
  cr_assert_eq(result, true);
}

Test(executor_conditions, LS_nc_z) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL); 
  cpu.cpsr = SET_Z;
 
  bool result = _petraea_eval_condition(&cpu, COND_LS);
  cr_assert_eq(result, true);
}

Test(executor_conditions, LS_c_nz) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL); 
  cpu.cpsr = SET_C;
 
  bool result = _petraea_eval_condition(&cpu, COND_LS);
  cr_assert_eq(result, false);
}

Test(executor_conditions, LS_c_z) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL); 
  cpu.cpsr = SET_C | SET_Z;
 
  bool result = _petraea_eval_condition(&cpu, COND_LS);
  cr_assert_eq(result, true);
}

Test(executor_conditions, GE_n_v) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL); 
  cpu.cpsr = SET_N | SET_V;
 
  bool result = _petraea_eval_condition(&cpu, COND_GE);
  cr_assert_eq(result, true);
}

Test(executor_conditions, GE_nn_nv) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL); 

  bool result = _petraea_eval_condition(&cpu, COND_GE);
  cr_assert_eq(result, true);
}

Test(executor_conditions, GE_nn_v) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL); 
  cpu.cpsr = SET_V;
 
  bool result = _petraea_eval_condition(&cpu, COND_GE);
  cr_assert_eq(result, false);
}

Test(executor_conditions, GE_n_nv) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL); 
  cpu.cpsr = SET_N;
 
  bool result = _petraea_eval_condition(&cpu, COND_GE);
  cr_assert_eq(result, false);
}

Test(executor_conditions, LT_n_v) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL); 
  cpu.cpsr = SET_N | SET_V;
 
  bool result = _petraea_eval_condition(&cpu, COND_LT);
  cr_assert_eq(result, false);
}

Test(executor_conditions, LT_nn_nv) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL); 

  bool result = _petraea_eval_condition(&cpu, COND_LT);
  cr_assert_eq(result, false);
}

Test(executor_conditions, LT_nn_v) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL); 
  cpu.cpsr = SET_V;
 
  bool result = _petraea_eval_condition(&cpu, COND_LT);
  cr_assert_eq(result, true);
}

Test(executor_conditions, LT_n_nv) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL); 
  cpu.cpsr = SET_N;
 
  bool result = _petraea_eval_condition(&cpu, COND_LT);
  cr_assert_eq(result, true);
}

Test(executor_conditions, AL) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL); 
 
  bool result = _petraea_eval_condition(&cpu, COND_AL);
  cr_assert_eq(result, true);
}

Test(executor_conditions, NV) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL); 
 
  bool result = _petraea_eval_condition(&cpu, COND_NV);
  cr_assert_eq(result, false);
}

Test(executor_conditions, GT_n_v_nz) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL); 
  cpu.cpsr = SET_N | SET_V;
 
  bool result = _petraea_eval_condition(&cpu, COND_GT);
  cr_assert_eq(result, true);
}

Test(executor_conditions, GT_nn_nv_nz) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL); 

  bool result = _petraea_eval_condition(&cpu, COND_GT);
  cr_assert_eq(result, true);
}

Test(executor_conditions, GT_nn_v_nz) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL); 
  cpu.cpsr = SET_V;
 
  bool result = _petraea_eval_condition(&cpu, COND_GT);
  cr_assert_eq(result, false);
}

Test(executor_conditions, GT_n_nv_nz) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL); 
  cpu.cpsr = SET_N;
 
  bool result = _petraea_eval_condition(&cpu, COND_GT);
  cr_assert_eq(result, false);
}

Test(executor_conditions, GT_n_v_z) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL); 
  cpu.cpsr = SET_N | SET_V | SET_Z;
 
  bool result = _petraea_eval_condition(&cpu, COND_GT);
  cr_assert_eq(result, false);
}

Test(executor_conditions, GT_nn_nv_z) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.cpsr = SET_Z;

  bool result = _petraea_eval_condition(&cpu, COND_GT);
  cr_assert_eq(result, false);
}

Test(executor_conditions, GT_nn_v_z) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL); 
  cpu.cpsr = SET_V | SET_Z;
 
  bool result = _petraea_eval_condition(&cpu, COND_GT);
  cr_assert_eq(result, false);
}

Test(executor_conditions, GT_n_nv_z) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL); 
  cpu.cpsr = SET_N | SET_Z;
 
  bool result = _petraea_eval_condition(&cpu, COND_GT);
  cr_assert_eq(result, false);
}

Test(executor_conditions, LE_n_v_nz) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL); 
  cpu.cpsr = SET_N | SET_V;
 
  bool result = _petraea_eval_condition(&cpu, COND_LE);
  cr_assert_eq(result, false);
}

Test(executor_conditions, LE_nn_nv_nz) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL); 

  bool result = _petraea_eval_condition(&cpu, COND_LE);
  cr_assert_eq(result, false);
}

Test(executor_conditions, LE_nn_v_nz) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL); 
  cpu.cpsr = SET_V;
 
  bool result = _petraea_eval_condition(&cpu, COND_LE);
  cr_assert_eq(result, false);
}

Test(executor_conditions, LE_n_nv_nz) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL); 
  cpu.cpsr = SET_N;
 
  bool result = _petraea_eval_condition(&cpu, COND_LE);
  cr_assert_eq(result, false);
}

Test(executor_conditions, LE_n_v_z) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL); 
  cpu.cpsr = SET_N | SET_V | SET_Z;
 
  bool result = _petraea_eval_condition(&cpu, COND_LE);
  cr_assert_eq(result, false);
}

Test(executor_conditions, LE_nn_nv_z) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.cpsr = SET_Z;

  bool result = _petraea_eval_condition(&cpu, COND_LE);
  cr_assert_eq(result, false);
}

Test(executor_conditions, LE_nn_v_z) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL); 
  cpu.cpsr = SET_V | SET_Z;
 
  bool result = _petraea_eval_condition(&cpu, COND_LE);
  cr_assert_eq(result, true);
}

Test(executor_conditions, LE_n_nv_z) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL); 
  cpu.cpsr = SET_N | SET_Z;
 
  bool result = _petraea_eval_condition(&cpu, COND_LE);
  cr_assert_eq(result, true);
}

///////////////////////////////////////////
// Single data transfer
///////////////////////////////////////////

Test(executor_single_data_transfer, read_halfword) {
  int bus_fetch(uint32_t address, int bytes, void* ret) {
    *((uint32_t*)ret) = (address == 0x13579240 ? 0x12345678 : 0x87654321);
    return 0;
  }
  
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, &bus_fetch, NULL); 

  pt_arm_instruction instr;
  instr.type = INSTR_SINGLE_DATA_TRANSFER;
  instr.cond = COND_AL;
  instr.instr.single_data_transfer.add_offset_before_transfer = true;
  instr.instr.single_data_transfer.add_offset = true;
  instr.instr.single_data_transfer.transfer_byte = false;
  instr.instr.single_data_transfer.write_back_address = false;
  instr.instr.single_data_transfer.load = true;
  instr.instr.single_data_transfer.base = REG_R1;
  instr.instr.single_data_transfer.source_dest = REG_R0;
  instr.instr.single_data_transfer.offset.is_immediate = true;
  instr.instr.single_data_transfer.offset.op.imm.value = 4;
  cpu.r1 = (0x13579240 - 4);
  
  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r0, 0x12345678);
  cr_assert_eq(cpu.r1, 0x13579240 - 4);
}

Test(executor_single_data_transfer, write_word) {
  uint32_t writtenValue = 0xDEADBEEF;
  int bus_write(uint32_t address, int bytes, void* value) {
    if(address == 0x13579240 && bytes == 4) {
      writtenValue = *((uint32_t*)value);
    }
    return 0;
  }
  
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, &bus_write); 

  pt_arm_instruction instr;
  instr.type = INSTR_SINGLE_DATA_TRANSFER;
  instr.cond = COND_AL;
  instr.instr.single_data_transfer.add_offset_before_transfer = true;
  instr.instr.single_data_transfer.add_offset = true;
  instr.instr.single_data_transfer.transfer_byte = false;
  instr.instr.single_data_transfer.write_back_address = false;
  instr.instr.single_data_transfer.load = false;
  instr.instr.single_data_transfer.base = REG_R1;
  instr.instr.single_data_transfer.source_dest = REG_R0;
  instr.instr.single_data_transfer.offset.is_immediate = true;
  instr.instr.single_data_transfer.offset.op.imm.value = 4;
  cpu.r0 = 0x12345678;
  cpu.r1 = (0x13579240 - 4);
  
  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(writtenValue, 0x12345678);
}

Test(executor_single_data_transfer, read_word_byte) {
  int bus_fetch(uint32_t address, int bytes, void* ret) {
    *((uint8_t*)ret) = (address == 0x13579240 ? 0x12 : 0x34);
    return 0;
  }
  
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, &bus_fetch, NULL);

  pt_arm_instruction instr;
  instr.type = INSTR_SINGLE_DATA_TRANSFER;
  instr.cond = COND_AL;
  instr.instr.single_data_transfer.add_offset_before_transfer = true;
  instr.instr.single_data_transfer.add_offset = true;
  instr.instr.single_data_transfer.transfer_byte = true;
  instr.instr.single_data_transfer.write_back_address = false;
  instr.instr.single_data_transfer.load = true;
  instr.instr.single_data_transfer.base = REG_R1;
  instr.instr.single_data_transfer.source_dest = REG_R0;
  instr.instr.single_data_transfer.offset.is_immediate = true;
  instr.instr.single_data_transfer.offset.op.imm.value = 4;
  cpu.r1 = (0x13579240 - 4);
  
  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r0, 0x12);
  cr_assert_eq(cpu.r1, 0x13579240 - 4);
}

Test(executor_single_data_transfer, write_byte) {
  uint8_t writtenValue = 0xDE;
  int bus_write(uint32_t address, int bytes, void* value) {
    if(address == 0x13579240 && bytes == 1) {
      writtenValue = *((uint8_t*)value);
    }
    return 0;
  }
  
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, &bus_write);

  pt_arm_instruction instr;
  instr.type = INSTR_SINGLE_DATA_TRANSFER;
  instr.cond = COND_AL;
  instr.instr.single_data_transfer.add_offset_before_transfer = true;
  instr.instr.single_data_transfer.add_offset = true;
  instr.instr.single_data_transfer.transfer_byte = true;
  instr.instr.single_data_transfer.write_back_address = false;
  instr.instr.single_data_transfer.load = false;
  instr.instr.single_data_transfer.base = REG_R1;
  instr.instr.single_data_transfer.source_dest = REG_R0;
  instr.instr.single_data_transfer.offset.is_immediate = true;
  instr.instr.single_data_transfer.offset.op.imm.value = 4;
  cpu.r0 = 0x12345678;
  cpu.r1 = (0x13579240 - 4);
  
  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(writtenValue, 0x78);
}

Test(executor_single_data_transfer, write_back_base) {
  int bus_fetch(uint32_t address, int bytes, void* ret) {
    *((uint32_t*)ret) = (address == 0x13579240 ? 0x12345678 : 0x87654321);
    return 0;
  }

  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, &bus_fetch, NULL);

  pt_arm_instruction instr;
  instr.type = INSTR_SINGLE_DATA_TRANSFER;
  instr.cond = COND_AL;
  instr.instr.single_data_transfer.add_offset_before_transfer = true;
  instr.instr.single_data_transfer.add_offset = true;
  instr.instr.single_data_transfer.transfer_byte = false;
  instr.instr.single_data_transfer.write_back_address = true;
  instr.instr.single_data_transfer.load = true;
  instr.instr.single_data_transfer.base = REG_R1;
  instr.instr.single_data_transfer.source_dest = REG_R0;
  instr.instr.single_data_transfer.offset.is_immediate = true;
  instr.instr.single_data_transfer.offset.op.imm.value = 4;
  cpu.r1 = (0x13579240 - 4);
  
  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r0, 0x12345678);
  cr_assert_eq(cpu.r1, 0x13579240);
}

Test(executor_single_data_transfer, offset_base_after_transfer) {
  int bus_fetch(uint32_t address, int bytes, void* ret) {
    *((uint32_t*)ret) = (address == 0x13579240 ? 0x12345678 : 0x87654321);
    return 0;
  }
  
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, &bus_fetch, NULL);

  pt_arm_instruction instr;
  instr.type = INSTR_SINGLE_DATA_TRANSFER;
  instr.cond = COND_AL;
  instr.instr.single_data_transfer.add_offset_before_transfer = false;
  instr.instr.single_data_transfer.add_offset = true;
  instr.instr.single_data_transfer.transfer_byte = false;
  instr.instr.single_data_transfer.write_back_address = true;
  instr.instr.single_data_transfer.load = true;
  instr.instr.single_data_transfer.base = REG_R1;
  instr.instr.single_data_transfer.source_dest = REG_R0;
  instr.instr.single_data_transfer.offset.is_immediate = true;
  instr.instr.single_data_transfer.offset.op.imm.value = 4;
  cpu.r1 = 0x13579240;
  
  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r0, 0x12345678);
  cr_assert_eq(cpu.r1, 0x13579240+4);
}

///////////////////////////////////////////
// Halfword transfer
///////////////////////////////////////////

Test(executor_halfword_data_transfer, read_halfword) {
  int bus_fetch(uint32_t address, int bytes, void* ret) {
    *((uint16_t*)ret) = (address == 0x13579240 ? 0x5678 : 0x4321);
    return 0;
  }
  
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, &bus_fetch, NULL);

  pt_arm_instruction instr;
  instr.type = INSTR_HALFWORD_DATA_TRANSFER;
  instr.cond = COND_AL;
  instr.instr.halfword_data_transfer.add_offset_before_transfer = true;
  instr.instr.halfword_data_transfer.add_offset = true;
  instr.instr.halfword_data_transfer.transfer_byte = false;
  instr.instr.halfword_data_transfer.is_signed = false;
  instr.instr.halfword_data_transfer.write_back_address = false;
  instr.instr.halfword_data_transfer.load = true;
  instr.instr.halfword_data_transfer.base = REG_R1;
  instr.instr.halfword_data_transfer.source_dest = REG_R0;
  instr.instr.halfword_data_transfer.is_immediate_offset = true;
  instr.instr.halfword_data_transfer.offset_imm = 4;
  cpu.r1 = (0x13579240 - 4);
  
  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r0, 0x5678);
  cr_assert_eq(cpu.r1, 0x13579240 - 4);
}

Test(executor_halfword_data_transfer, read_halfword_subtract_offset) {
  int bus_fetch(uint32_t address, int bytes, void* ret) {
    *((uint16_t*)ret) = (address == 0x13579240 ? 0x5678 : 0x4321);
    return 0;
  }
  
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, &bus_fetch, NULL);

  pt_arm_instruction instr;
  instr.type = INSTR_HALFWORD_DATA_TRANSFER;
  instr.cond = COND_AL;
  instr.instr.halfword_data_transfer.add_offset_before_transfer = true;
  instr.instr.halfword_data_transfer.add_offset = false;
  instr.instr.halfword_data_transfer.transfer_byte = false;
  instr.instr.halfword_data_transfer.is_signed = false;
  instr.instr.halfword_data_transfer.write_back_address = false;
  instr.instr.halfword_data_transfer.load = true;
  instr.instr.halfword_data_transfer.base = REG_R1;
  instr.instr.halfword_data_transfer.source_dest = REG_R0;
  instr.instr.halfword_data_transfer.is_immediate_offset = true;
  instr.instr.halfword_data_transfer.offset_imm = 4;
  cpu.r1 = (0x13579240 + 4);
  
  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r0, 0x5678);
  cr_assert_eq(cpu.r1, 0x13579240 + 4);
}

Test(executor_halfword_data_transfer, read_halfword_reg_offset) {
  int bus_fetch(uint32_t address, int bytes, void* ret) {
    *((uint16_t*)ret) = (address == 0x13579240 ? 0x5678 : 0x4321);
    return 0;
  }
  
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, &bus_fetch, NULL);

  pt_arm_instruction instr;
  instr.type = INSTR_HALFWORD_DATA_TRANSFER;
  instr.cond = COND_AL;
  instr.instr.halfword_data_transfer.add_offset_before_transfer = true;
  instr.instr.halfword_data_transfer.add_offset = true;
  instr.instr.halfword_data_transfer.transfer_byte = false;
  instr.instr.halfword_data_transfer.is_signed = false;
  instr.instr.halfword_data_transfer.write_back_address = false;
  instr.instr.halfword_data_transfer.load = true;
  instr.instr.halfword_data_transfer.base = REG_R1;
  instr.instr.halfword_data_transfer.source_dest = REG_R0;
  instr.instr.halfword_data_transfer.is_immediate_offset = false;
  instr.instr.halfword_data_transfer.offset_reg = REG_R2;
  cpu.r1 = (0x13579240 - 4);
  cpu.r2 = 4;
  
  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r0, 0x5678);
  cr_assert_eq(cpu.r1, 0x13579240 - 4);
}

Test(executor_halfword_data_transfer, read_halfword_post_indexing) {
  int bus_fetch(uint32_t address, int bytes, void* ret) {
    *((uint16_t*)ret) = (address == 0x13579240 ? 0x5678 : 0x4321);
    return 0;
  }

  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, &bus_fetch, NULL);

  pt_arm_instruction instr;
  instr.type = INSTR_HALFWORD_DATA_TRANSFER;
  instr.cond = COND_AL;
  instr.instr.halfword_data_transfer.add_offset_before_transfer = false;
  instr.instr.halfword_data_transfer.add_offset = true;
  instr.instr.halfword_data_transfer.transfer_byte = false;
  instr.instr.halfword_data_transfer.is_signed = false;
  instr.instr.halfword_data_transfer.write_back_address = true;
  instr.instr.halfword_data_transfer.load = true;
  instr.instr.halfword_data_transfer.base = REG_R1;
  instr.instr.halfword_data_transfer.source_dest = REG_R0;
  instr.instr.halfword_data_transfer.is_immediate_offset = true;
  instr.instr.halfword_data_transfer.offset_imm = 4;
  cpu.r1 = 0x13579240;
  
  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r0, 0x5678);
  cr_assert_eq(cpu.r1, 0x13579240+4);
}

Test(executor_halfword_data_transfer, read_halfword_write_back) {
  int bus_fetch(uint32_t address, int bytes, void* ret) {
    *((uint16_t*)ret) = (address == 0x13579240 ? 0x5678 : 0x4321);
    return 0;
  }
  
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, &bus_fetch, NULL);

  pt_arm_instruction instr;
  instr.type = INSTR_HALFWORD_DATA_TRANSFER;
  instr.cond = COND_AL;
  instr.instr.halfword_data_transfer.add_offset_before_transfer = true;
  instr.instr.halfword_data_transfer.add_offset = true;
  instr.instr.halfword_data_transfer.transfer_byte = false;
  instr.instr.halfword_data_transfer.is_signed = false;
  instr.instr.halfword_data_transfer.write_back_address = true;
  instr.instr.halfword_data_transfer.load = true;
  instr.instr.halfword_data_transfer.base = REG_R1;
  instr.instr.halfword_data_transfer.source_dest = REG_R0;
  instr.instr.halfword_data_transfer.is_immediate_offset = true;
  instr.instr.halfword_data_transfer.offset_imm = 4;
  cpu.r1 = (0x13579240 - 4);
  
  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r0, 0x5678);
  cr_assert_eq(cpu.r1, 0x13579240);
}

Test(executor_halfword_data_transfer, read_halfword_signed_positive) {
  int bus_fetch(uint32_t address, int bytes, void* ret) {
    *((uint16_t*)ret) = (address == 0x13579240 ? 0x5678 : 0x4321);
    return 0;
  }
  
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, &bus_fetch, NULL);

  pt_arm_instruction instr;
  instr.type = INSTR_HALFWORD_DATA_TRANSFER;
  instr.cond = COND_AL;
  instr.instr.halfword_data_transfer.add_offset_before_transfer = true;
  instr.instr.halfword_data_transfer.add_offset = true;
  instr.instr.halfword_data_transfer.transfer_byte = false;
  instr.instr.halfword_data_transfer.is_signed = true;
  instr.instr.halfword_data_transfer.write_back_address = false;
  instr.instr.halfword_data_transfer.load = true;
  instr.instr.halfword_data_transfer.base = REG_R1;
  instr.instr.halfword_data_transfer.source_dest = REG_R0;
  instr.instr.halfword_data_transfer.is_immediate_offset = true;
  instr.instr.halfword_data_transfer.offset_imm = 4;
  cpu.r1 = (0x13579240 - 4);
  
  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r0, 0x5678);
  cr_assert_eq(cpu.r1, 0x13579240 - 4);
}

Test(executor_halfword_data_transfer, read_halfword_signed_negative) {
  int bus_fetch(uint32_t address, int bytes, void* ret) {
    *((uint16_t*)ret) = (address == 0x13579240 ? 0xF678 : 0x4321);
    return 0;
  }
  
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, &bus_fetch, NULL);

  pt_arm_instruction instr;
  instr.type = INSTR_HALFWORD_DATA_TRANSFER;
  instr.cond = COND_AL;
  instr.instr.halfword_data_transfer.add_offset_before_transfer = true;
  instr.instr.halfword_data_transfer.add_offset = true;
  instr.instr.halfword_data_transfer.transfer_byte = false;
  instr.instr.halfword_data_transfer.is_signed = true;
  instr.instr.halfword_data_transfer.write_back_address = false;
  instr.instr.halfword_data_transfer.load = true;
  instr.instr.halfword_data_transfer.base = REG_R1;
  instr.instr.halfword_data_transfer.source_dest = REG_R0;
  instr.instr.halfword_data_transfer.is_immediate_offset = true;
  instr.instr.halfword_data_transfer.offset_imm = 4;
  cpu.r1 = (0x13579240 - 4);
  
  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r0, 0xFFFFF678);
  cr_assert_eq(cpu.r1, 0x13579240 - 4);
}

Test(executor_halfword_data_transfer, write_halfword) {
  uint16_t writtenValue = 0xBEEF;
  int bus_write(uint32_t address, int bytes, void* value) {
    if(address == 0x13579240 && bytes == 2) {
      writtenValue = *((uint16_t*)value);
    }
    return 0;
  }
  
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, &bus_write);

  pt_arm_instruction instr;
  instr.type = INSTR_HALFWORD_DATA_TRANSFER;
  instr.cond = COND_AL;
  instr.instr.halfword_data_transfer.add_offset_before_transfer = true;
  instr.instr.halfword_data_transfer.add_offset = true;
  instr.instr.halfword_data_transfer.transfer_byte = false;
  instr.instr.halfword_data_transfer.is_signed = false;
  instr.instr.halfword_data_transfer.write_back_address = false;
  instr.instr.halfword_data_transfer.load = false;
  instr.instr.halfword_data_transfer.base = REG_R1;
  instr.instr.halfword_data_transfer.source_dest = REG_R0;
  instr.instr.halfword_data_transfer.is_immediate_offset = true;
  instr.instr.halfword_data_transfer.offset_imm = 4;
  cpu.r0 = 0x1234;
  cpu.r1 = (0x13579240 - 4);
  
  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(writtenValue, 0x1234);
  cr_assert_eq(cpu.r1, 0x13579240 - 4);
}

///////////////////////////////////////////
// Swap
///////////////////////////////////////////

Test(executor_swap, swap_word) {
  uint32_t writtenValue = 0xDEADBEEF;
  int bus_write(uint32_t address, int bytes, void* value) {
    if(address == 0x13579240) {
      writtenValue = *((uint32_t*)value);
    }
    return 0;
  }

  int bus_fetch(uint32_t address, int bytes, void* ret) {
    *((uint32_t*)ret) = (address == 0x13579240 ? 0x12345678 : 0x87654321);
    return 0;
  }
  
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, &bus_fetch, &bus_write); 

  pt_arm_instruction instr;
  instr.type = INSTR_SWAP;
  instr.cond = COND_AL;
  instr.instr.swap.transfer_byte = false;
  instr.instr.swap.addr = REG_R0;
  instr.instr.swap.value = REG_R1;
  instr.instr.swap.dest = REG_R2;
  cpu.r0 = 0x13579240;
  cpu.r1 = 0x11223344;
  cpu.r2 = 0xDEADBEEF;
  
  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r0, 0x13579240);
  cr_assert_eq(cpu.r1, 0x11223344);
  cr_assert_eq(cpu.r2, 0x12345678);
  cr_assert_eq(writtenValue, 0x11223344);
}

Test(executor_swap, swap_word_rrot8) {
  uint32_t writtenValue = 0xDEADBEEF;
  int bus_write(uint32_t address, int bytes, void* value) {
    if(address == 0x13579241 && bytes == 4) {
      writtenValue = *((uint32_t*)value);
    }
    return 0;
  }

  int bus_fetch(uint32_t address, int bytes, void* ret) {
    *((uint32_t*)ret) = (address == 0x13579241 ? 0x12345678 : 0x87654321);
    return 0;
  }
  
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, &bus_fetch, &bus_write);

  pt_arm_instruction instr;
  instr.type = INSTR_SWAP;
  instr.cond = COND_AL;
  instr.instr.swap.transfer_byte = false;
  instr.instr.swap.addr = REG_R0;
  instr.instr.swap.value = REG_R1;
  instr.instr.swap.dest = REG_R2;
  cpu.r0 = 0x13579241;
  cpu.r1 = 0x11223344;
  cpu.r2 = 0xDEADBEEF;
  
  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r0, 0x13579241);
  cr_assert_eq(cpu.r1, 0x11223344);
  cr_assert_eq(cpu.r2, 0x78123456);
  cr_assert_eq(writtenValue, 0x11223344);
}

Test(executor_swap, swap_word_rrot16) {
  uint32_t writtenValue = 0xDEADBEEF;
  int bus_write(uint32_t address, int bytes, void* value) {
    if(address == 0x13579242 && bytes == 4) {
      writtenValue = *((uint32_t*)value);
    }
    return 0;
  }

  int bus_fetch(uint32_t address, int bytes, void* ret) {
    *((uint32_t*)ret) = (address == 0x13579242 ? 0x12345678 : 0x87654321);
    return 0;
  }

  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, &bus_fetch, &bus_write);

  pt_arm_instruction instr;
  instr.type = INSTR_SWAP;
  instr.cond = COND_AL;
  instr.instr.swap.transfer_byte = false;
  instr.instr.swap.addr = REG_R0;
  instr.instr.swap.value = REG_R1;
  instr.instr.swap.dest = REG_R2;
  cpu.r0 = 0x13579242;
  cpu.r1 = 0x11223344;
  cpu.r2 = 0xDEADBEEF;
  
  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r0, 0x13579242);
  cr_assert_eq(cpu.r1, 0x11223344);
  cr_assert_eq(cpu.r2, 0x56781234);
  cr_assert_eq(writtenValue, 0x11223344);
}

Test(executor_swap, swap_word_rrot24) {
  uint32_t writtenValue = 0xDEADBEEF;
  int bus_write(uint32_t address, int bytes, void* value) {
    if(address == 0x13579243 && bytes == 4) {
      writtenValue = *((uint32_t*)value);
    }
    return 0;
  }

  int bus_fetch(uint32_t address, int bytes, void* ret) {
    *((uint32_t*)ret) = (address == 0x13579243 ? 0x12345678 : 0x87654321);
    return 0;
  }

  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, &bus_fetch, &bus_write);

  pt_arm_instruction instr;
  instr.type = INSTR_SWAP;
  instr.cond = COND_AL;
  instr.instr.swap.transfer_byte = false;
  instr.instr.swap.addr = REG_R0;
  instr.instr.swap.value = REG_R1;
  instr.instr.swap.dest = REG_R2;
  cpu.r0 = 0x13579243;
  cpu.r1 = 0x11223344;
  cpu.r2 = 0xDEADBEEF;
  
  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r0, 0x13579243);
  cr_assert_eq(cpu.r1, 0x11223344);
  cr_assert_eq(cpu.r2, 0x34567812);
  cr_assert_eq(writtenValue, 0x11223344);
}

Test(executor_swap, swap_byte) {
  uint8_t writtenValue = 0xEF;
  int bus_write(uint32_t address, int bytes, void* value) {
    if(address == 0x13579240 && bytes == 1) {
      writtenValue = *((uint8_t*)value);
    }
    return 0;
  }

  int bus_fetch(uint32_t address, int bytes, void* ret) {
    *((uint8_t*)ret) = (address == 0x13579240 ? 0x12 : 021);
    return 0;
  }
  
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, &bus_fetch, &bus_write);

  pt_arm_instruction instr;
  instr.type = INSTR_SWAP;
  instr.cond = COND_AL;
  instr.instr.swap.transfer_byte = true;
  instr.instr.swap.addr = REG_R0;
  instr.instr.swap.value = REG_R1;
  instr.instr.swap.dest = REG_R2;
  cpu.r0 = 0x13579240;
  cpu.r1 = 0xFA;
  cpu.r2 = 0xFF;
  
  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r0, 0x13579240);
  cr_assert_eq(cpu.r1, 0xFA);
  cr_assert_eq(cpu.r2, 0x12);
  cr_assert_eq(writtenValue, 0xFA);
}

///////////////////////////////////////////
// Coprocessor register transfer
///////////////////////////////////////////

Test(executor_cp_reg_transfer, unknown_coprocessor) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL); 

  pt_arm_instruction instr;
  instr.type = INSTR_COPROCESSOR_REGISTER_TRANSFER;
  instr.cond = COND_AL;
  instr.instr.coprocessor_register_transfer.load = true;
  instr.instr.coprocessor_register_transfer.source_dest = REG_R3;
  instr.instr.coprocessor_register_transfer.cp_reg = 2;
  instr.instr.coprocessor_register_transfer.cp_num = 1;
  instr.instr.coprocessor_register_transfer.opcode_2 = 3;
  instr.instr.coprocessor_register_transfer.crm = 4;
  
  cr_assert_eq(pt_arm_execute_instruction(&cpu, &instr), -1);
}

Test(executor_cp_reg_transfer, read_non_r15) {
  uint32_t read(pt_arm_cpu* cpu, void* state, uint8_t cp_reg, uint8_t crm, uint8_t opcode2) {
    if(cp_reg == 2 && crm == 4 && opcode2 == 3) {
      return 0x12345678;
    }
    return 0x0;
  }
  
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL); 

  pt_arm_instruction instr;
  instr.type = INSTR_COPROCESSOR_REGISTER_TRANSFER;
  instr.cond = COND_AL;
  instr.instr.coprocessor_register_transfer.load = true;
  instr.instr.coprocessor_register_transfer.source_dest = REG_R3;
  instr.instr.coprocessor_register_transfer.cp_reg = 2;
  instr.instr.coprocessor_register_transfer.cp_num = 1;
  instr.instr.coprocessor_register_transfer.opcode_2 = 3;
  instr.instr.coprocessor_register_transfer.crm = 4;

  cpu.coprocessors[1].present = true;
  cpu.coprocessors[1].read = &read;

  int r = pt_arm_execute_instruction(&cpu, &instr);
  cr_assert_eq(r, 0);
  cr_assert_eq(cpu.r3, 0x12345678);
}

Test(executor_cp_reg_transfer, read_r15) {
  uint32_t read(pt_arm_cpu* cpu, void* state, uint8_t cp_reg, uint8_t crm, uint8_t opcode2) {
    if(cp_reg == 2 && crm == 4 && opcode2 == 3) {
      return 0xA0000000;
    } else if(cp_reg == 2 && crm == 4 && opcode2 == 4) {
      return 0x50000000;
    }
    
    return 0x0;
  }
  
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL);
  cpu.r15 = 0x13243546;

  pt_arm_instruction instr;
  instr.type = INSTR_COPROCESSOR_REGISTER_TRANSFER;
  instr.cond = COND_AL;
  instr.instr.coprocessor_register_transfer.load = true;
  instr.instr.coprocessor_register_transfer.source_dest = REG_R15;
  instr.instr.coprocessor_register_transfer.cp_reg = 2;
  instr.instr.coprocessor_register_transfer.cp_num = 1;
  instr.instr.coprocessor_register_transfer.opcode_2 = 3;
  instr.instr.coprocessor_register_transfer.crm = 4;

  cpu.coprocessors[1].present = true;
  cpu.coprocessors[1].read = &read;

  int r = pt_arm_execute_instruction(&cpu, &instr);
  cr_assert_eq(r, 0);
  cr_assert_eq(cpu.r15, 0x13243546);
  cr_assert_eq(GET_NEGATIVE_FLAG(&cpu), true);
  cr_assert_eq(GET_ZERO_FLAG(&cpu), false);
  cr_assert_eq(GET_CARRY_FLAG(&cpu), true);
  cr_assert_eq(GET_OVERFLOW_FLAG(&cpu), false);

  instr.instr.coprocessor_register_transfer.opcode_2 = 4;
  pt_arm_execute_instruction(&cpu, &instr);
  cr_assert_eq(cpu.r15, 0x13243546);
  cr_assert_eq(GET_NEGATIVE_FLAG(&cpu), false);
  cr_assert_eq(GET_ZERO_FLAG(&cpu), true);
  cr_assert_eq(GET_CARRY_FLAG(&cpu), false);
  cr_assert_eq(GET_OVERFLOW_FLAG(&cpu), true);
}

Test(executor_cp_reg_transfer, write) {
  uint32_t written = 0x0;
  void write(pt_arm_cpu* cpu, void* state, uint8_t cp_reg, uint32_t value, uint8_t crm, uint8_t opcode2) {
    if(cp_reg == 2 && crm == 4 && opcode2 == 3) {
      written = value;
    }
  }
  
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL); 

  pt_arm_instruction instr;
  instr.type = INSTR_COPROCESSOR_REGISTER_TRANSFER;
  instr.cond = COND_AL;
  instr.instr.coprocessor_register_transfer.load = false;
  instr.instr.coprocessor_register_transfer.source_dest = REG_R3;
  instr.instr.coprocessor_register_transfer.cp_reg = 2;
  instr.instr.coprocessor_register_transfer.cp_num = 1;
  instr.instr.coprocessor_register_transfer.opcode_2 = 3;
  instr.instr.coprocessor_register_transfer.crm = 4;

  cpu.coprocessors[1].present = true;
  cpu.coprocessors[1].write = &write;

  cpu.r3 = 0x98765432;

  int r = pt_arm_execute_instruction(&cpu, &instr);
  cr_assert_eq(r, 0);
  cr_assert_eq(written, 0x98765432);
}


