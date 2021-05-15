#include <stdlib.h>
#include <stdio.h>
#include <criterion/criterion.h>
#include "instr.h"

Test(decoder, can_decode_condition) {  
  __arm_instruction result;
  arm_decode_instruction(&result, 0xc0800001); // ADDGT R0,R0,R1
  cr_assert_eq(result.cond, COND_GT);
}

Test(decoder, can_decode_a_data_processing_instr) {
  __arm_instruction result;
  arm_decode_instruction(&result, 0xE0430001); // SUB R0,R3,R1
  cr_assert_eq(result.type, INSTR_DATA_PROCESSING);
  cr_assert_eq(result.instr.data_processing.opcode, OPERAND_SUB);
  cr_assert_eq(result.instr.data_processing.set_condition_codes, false);
  cr_assert_eq(result.instr.data_processing.operand1, REG_R3);
}

Test(decoder, can_decode_a_data_processing_instr_updating_condition_flags) {
  __arm_instruction result;
  arm_decode_instruction(&result, 0xE0530001); // SUBS R0,R3,R1
  cr_assert_eq(result.type, INSTR_DATA_PROCESSING);
  cr_assert_eq(result.instr.data_processing.opcode, OPERAND_SUB);
  cr_assert_eq(result.instr.data_processing.set_condition_codes, true);
  cr_assert_eq(result.instr.data_processing.operand1, REG_R3);
}

Test(decoder, can_decode_a_data_processing_instr_with_operand2_immediate) {
  __arm_instruction result;
  arm_decode_instruction(&result, 0xE243002D); // SUB R0,R3,#45
  cr_assert_eq(result.type, INSTR_DATA_PROCESSING);
  cr_assert_eq(result.instr.data_processing.opcode, OPERAND_SUB);
  cr_assert_eq(result.instr.data_processing.operand1, REG_R3);
  cr_assert_eq(result.instr.data_processing.operand2.is_immediate, true);
  cr_assert_eq(result.instr.data_processing.operand2.op.imm.value, 45);
}

Test(decoder, can_decode_a_data_processing_instr_with_operand2_immediate_rotated) {
  __arm_instruction result;
  arm_decode_instruction(&result, 0xE24304FF); // SUB R0,R3,#0xFF000000
  cr_assert_eq(result.type, INSTR_DATA_PROCESSING);
  cr_assert_eq(result.instr.data_processing.opcode, OPERAND_SUB);
  cr_assert_eq(result.instr.data_processing.operand1, REG_R3);
  cr_assert_eq(result.instr.data_processing.operand2.is_immediate, true);
  cr_assert_eq(result.instr.data_processing.operand2.op.imm.value, 0xFF000000);
}

Test(decoder, can_decode_an_undefined_instruction) {
  __arm_instruction result;
  arm_decode_instruction(&result, 0x46012516);
  cr_assert_eq(result.type, INSTR_UNDEFINED);
}

