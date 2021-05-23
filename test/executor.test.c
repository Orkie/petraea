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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
  bool carryValid;
  bool carry;

  pt_arm_operand2 op;
  op.is_immediate = true;
  op.op.imm.value = 0x123456;
  
  uint32_t result = _petraea_eval_operand2(&cpu, &op, &carryValid, &carry);

  cr_assert_eq(result, 0x123456);
  cr_assert_eq(carryValid, true);
  cr_assert_eq(carry, false);
}

Test(_petraea_eval_operand2, can_evaluate_register_lsl0) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
  cpu.r14 = 0xDEADBEEF;
  cpu.r15 = 0x24;
  pt_arm_instruction instr;
  instr.type = INSTR_BRANCH;
  instr.cond = COND_AL;
  instr.instr.branch.link = true;
  instr.instr.branch.offset = 8;
  
  pt_arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r14, 0x24);
  cr_assert_eq(cpu.r15, 0x24+8);
}

///////////////////////////////////////////
// AND
///////////////////////////////////////////

Test(executor_and, can_execute_and_nc_nz_nn) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
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
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
  
  bool result = _petraea_eval_condition(&cpu, COND_EQ);
  cr_assert_eq(result, false);
}

Test(executor_conditions, EQ_z) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
  cpu.cpsr = SET_Z;
  
  bool result = _petraea_eval_condition(&cpu, COND_EQ);
  cr_assert_eq(result, true);
}

Test(executor_conditions, NE_nz) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
  
  bool result = _petraea_eval_condition(&cpu, COND_NE);
  cr_assert_eq(result, true);
}

Test(executor_conditions, NE_z) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
  cpu.cpsr = SET_Z;
  
  bool result = _petraea_eval_condition(&cpu, COND_NE);
  cr_assert_eq(result, false);
}

Test(executor_conditions, CS_nc) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
  
  bool result = _petraea_eval_condition(&cpu, COND_CS);
  cr_assert_eq(result, false);
}

Test(executor_conditions, CS_c) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
  cpu.cpsr = SET_C;
  
  bool result = _petraea_eval_condition(&cpu, COND_CS);
  cr_assert_eq(result, true);
}

Test(executor_conditions, CC_nc) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
  
  bool result = _petraea_eval_condition(&cpu, COND_CC);
  cr_assert_eq(result, true);
}

Test(executor_conditions, CC_c) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
  cpu.cpsr = SET_C;
  
  bool result = _petraea_eval_condition(&cpu, COND_CC);
  cr_assert_eq(result, false);
}

Test(executor_conditions, MI_nn) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
  
  bool result = _petraea_eval_condition(&cpu, COND_MI);
  cr_assert_eq(result, false);
}

Test(executor_conditions, MI_n) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
  cpu.cpsr = SET_N;
  
  bool result = _petraea_eval_condition(&cpu, COND_MI);
  cr_assert_eq(result, true);
}

Test(executor_conditions, PL_nn) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
  
  bool result = _petraea_eval_condition(&cpu, COND_PL);
  cr_assert_eq(result, true);
}

Test(executor_conditions, PL_n) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
  cpu.cpsr = SET_N;
  
  bool result = _petraea_eval_condition(&cpu, COND_PL);
  cr_assert_eq(result, false);
}

Test(executor_conditions, VS_nv) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
  
  bool result = _petraea_eval_condition(&cpu, COND_VS);
  cr_assert_eq(result, false);
}

Test(executor_conditions, VS_v) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
  cpu.cpsr = SET_V;
  
  bool result = _petraea_eval_condition(&cpu, COND_VS);
  cr_assert_eq(result, true);
}

Test(executor_conditions, VC_nv) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
  
  bool result = _petraea_eval_condition(&cpu, COND_VC);
  cr_assert_eq(result, true);
}

Test(executor_conditions, VC_v) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
  cpu.cpsr = SET_V;
  
  bool result = _petraea_eval_condition(&cpu, COND_VC);
  cr_assert_eq(result, false);
}

Test(executor_conditions, HI_c_nz) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
  cpu.cpsr = SET_C;
  
  bool result = _petraea_eval_condition(&cpu, COND_HI);
  cr_assert_eq(result, true);
}

Test(executor_conditions, HI_c_z) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL); 
  cpu.cpsr = SET_C | SET_Z;
 
  bool result = _petraea_eval_condition(&cpu, COND_HI);
  cr_assert_eq(result, false);
}

Test(executor_conditions, HI_nc_nz) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL); 
 
  bool result = _petraea_eval_condition(&cpu, COND_HI);
  cr_assert_eq(result, false);
}

Test(executor_conditions, LS_nc_nz) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL); 
 
  bool result = _petraea_eval_condition(&cpu, COND_LS);
  cr_assert_eq(result, true);
}

Test(executor_conditions, LS_nc_z) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL); 
  cpu.cpsr = SET_Z;
 
  bool result = _petraea_eval_condition(&cpu, COND_LS);
  cr_assert_eq(result, true);
}

Test(executor_conditions, LS_c_nz) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL); 
  cpu.cpsr = SET_C;
 
  bool result = _petraea_eval_condition(&cpu, COND_LS);
  cr_assert_eq(result, false);
}

Test(executor_conditions, LS_c_z) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL); 
  cpu.cpsr = SET_C | SET_Z;
 
  bool result = _petraea_eval_condition(&cpu, COND_LS);
  cr_assert_eq(result, true);
}

Test(executor_conditions, GE_n_v) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL); 
  cpu.cpsr = SET_N | SET_V;
 
  bool result = _petraea_eval_condition(&cpu, COND_GE);
  cr_assert_eq(result, true);
}

Test(executor_conditions, GE_nn_nv) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL); 

  bool result = _petraea_eval_condition(&cpu, COND_GE);
  cr_assert_eq(result, true);
}

Test(executor_conditions, GE_nn_v) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL); 
  cpu.cpsr = SET_V;
 
  bool result = _petraea_eval_condition(&cpu, COND_GE);
  cr_assert_eq(result, false);
}

Test(executor_conditions, GE_n_nv) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL); 
  cpu.cpsr = SET_N;
 
  bool result = _petraea_eval_condition(&cpu, COND_GE);
  cr_assert_eq(result, false);
}

Test(executor_conditions, LT_n_v) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL); 
  cpu.cpsr = SET_N | SET_V;
 
  bool result = _petraea_eval_condition(&cpu, COND_LT);
  cr_assert_eq(result, false);
}

Test(executor_conditions, LT_nn_nv) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL); 

  bool result = _petraea_eval_condition(&cpu, COND_LT);
  cr_assert_eq(result, false);
}

Test(executor_conditions, LT_nn_v) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL); 
  cpu.cpsr = SET_V;
 
  bool result = _petraea_eval_condition(&cpu, COND_LT);
  cr_assert_eq(result, true);
}

Test(executor_conditions, LT_n_nv) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL); 
  cpu.cpsr = SET_N;
 
  bool result = _petraea_eval_condition(&cpu, COND_LT);
  cr_assert_eq(result, true);
}

Test(executor_conditions, AL) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL); 
 
  bool result = _petraea_eval_condition(&cpu, COND_AL);
  cr_assert_eq(result, true);
}

Test(executor_conditions, NV) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL); 
 
  bool result = _petraea_eval_condition(&cpu, COND_NV);
  cr_assert_eq(result, false);
}

Test(executor_conditions, GT_n_v_nz) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL); 
  cpu.cpsr = SET_N | SET_V;
 
  bool result = _petraea_eval_condition(&cpu, COND_GT);
  cr_assert_eq(result, true);
}

Test(executor_conditions, GT_nn_nv_nz) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL); 

  bool result = _petraea_eval_condition(&cpu, COND_GT);
  cr_assert_eq(result, true);
}

Test(executor_conditions, GT_nn_v_nz) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL); 
  cpu.cpsr = SET_V;
 
  bool result = _petraea_eval_condition(&cpu, COND_GT);
  cr_assert_eq(result, false);
}

Test(executor_conditions, GT_n_nv_nz) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL); 
  cpu.cpsr = SET_N;
 
  bool result = _petraea_eval_condition(&cpu, COND_GT);
  cr_assert_eq(result, false);
}

Test(executor_conditions, GT_n_v_z) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL); 
  cpu.cpsr = SET_N | SET_V | SET_Z;
 
  bool result = _petraea_eval_condition(&cpu, COND_GT);
  cr_assert_eq(result, false);
}

Test(executor_conditions, GT_nn_nv_z) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
  cpu.cpsr = SET_Z;

  bool result = _petraea_eval_condition(&cpu, COND_GT);
  cr_assert_eq(result, false);
}

Test(executor_conditions, GT_nn_v_z) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL); 
  cpu.cpsr = SET_V | SET_Z;
 
  bool result = _petraea_eval_condition(&cpu, COND_GT);
  cr_assert_eq(result, false);
}

Test(executor_conditions, GT_n_nv_z) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL); 
  cpu.cpsr = SET_N | SET_Z;
 
  bool result = _petraea_eval_condition(&cpu, COND_GT);
  cr_assert_eq(result, false);
}

Test(executor_conditions, LE_n_v_nz) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL); 
  cpu.cpsr = SET_N | SET_V;
 
  bool result = _petraea_eval_condition(&cpu, COND_LE);
  cr_assert_eq(result, false);
}

Test(executor_conditions, LE_nn_nv_nz) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL); 

  bool result = _petraea_eval_condition(&cpu, COND_LE);
  cr_assert_eq(result, false);
}

Test(executor_conditions, LE_nn_v_nz) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL); 
  cpu.cpsr = SET_V;
 
  bool result = _petraea_eval_condition(&cpu, COND_LE);
  cr_assert_eq(result, false);
}

Test(executor_conditions, LE_n_nv_nz) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL); 
  cpu.cpsr = SET_N;
 
  bool result = _petraea_eval_condition(&cpu, COND_LE);
  cr_assert_eq(result, false);
}

Test(executor_conditions, LE_n_v_z) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL); 
  cpu.cpsr = SET_N | SET_V | SET_Z;
 
  bool result = _petraea_eval_condition(&cpu, COND_LE);
  cr_assert_eq(result, false);
}

Test(executor_conditions, LE_nn_nv_z) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL);
  cpu.cpsr = SET_Z;

  bool result = _petraea_eval_condition(&cpu, COND_LE);
  cr_assert_eq(result, false);
}

Test(executor_conditions, LE_nn_v_z) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL); 
  cpu.cpsr = SET_V | SET_Z;
 
  bool result = _petraea_eval_condition(&cpu, COND_LE);
  cr_assert_eq(result, true);
}

Test(executor_conditions, LE_n_nv_z) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, NULL, NULL, NULL); 
  cpu.cpsr = SET_N | SET_Z;
 
  bool result = _petraea_eval_condition(&cpu, COND_LE);
  cr_assert_eq(result, true);
}

