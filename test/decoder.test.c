#include <stdlib.h>
#include <stdio.h>
#include <criterion/criterion.h>
#include "instr.h"

///////////////////////////////////////////
// Conditions
///////////////////////////////////////////

Test(decoder, can_decode_condition) {  
  __arm_instruction result;
  arm_decode_instruction(&result, 0xc0800001); // ADDGT R0,R0,R1
  cr_assert_eq(result.cond, COND_GT);
}

///////////////////////////////////////////
// Data processing
///////////////////////////////////////////

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

Test(decoder, can_decode_a_data_processing_instr_with_operand2_register) {
  __arm_instruction result;
  arm_decode_instruction(&result, 0xE0430004); // SUB R0,R3,R4
  cr_assert_eq(result.type, INSTR_DATA_PROCESSING);
  cr_assert_eq(result.instr.data_processing.opcode, OPERAND_SUB);
  cr_assert_eq(result.instr.data_processing.operand1, REG_R3);
  cr_assert_eq(result.instr.data_processing.operand2.is_immediate, false);
  cr_assert_eq(result.instr.data_processing.operand2.op.reg.reg, REG_R4);
}

///////////////////////////////////////////
// Undefined
///////////////////////////////////////////

Test(decoder, can_decode_an_undefined_instruction) {
  __arm_instruction result;
  arm_decode_instruction(&result, 0x46012516);
  cr_assert_eq(result.type, INSTR_UNDEFINED);
}

///////////////////////////////////////////
// Single data transfer
///////////////////////////////////////////

Test(decoder, can_decode_a_simple_ldr) {
  __arm_instruction result;
  arm_decode_instruction(&result, 0xE51F0004); // LDR R0,=0x12345678
  cr_assert_eq(result.type, INSTR_SINGLE_DATA_TRANSFER);
  cr_assert_eq(result.instr.single_data_transfer.add_offset_before_transfer, true);
  cr_assert_eq(result.instr.single_data_transfer.add_offset_before_transfer, true);
  cr_assert_eq(result.instr.single_data_transfer.transfer_byte, false);
  cr_assert_eq(result.instr.single_data_transfer.write_back_address, false);
  cr_assert_eq(result.instr.single_data_transfer.load, true);

  // The above instruction is assembled as the instr word followed by the value, so it loads
  // the value at offset 4 from PC (i.e. the word after the instruction we are decoding)
  cr_assert_eq(result.instr.single_data_transfer.base, REG_PC);
  cr_assert_eq(result.instr.single_data_transfer.source_dest, REG_R0);
  cr_assert_eq(result.instr.single_data_transfer.offset.is_immediate, true);
  cr_assert_eq(result.instr.single_data_transfer.offset.op.imm.value, 4);
}

Test(decoder, can_decode_an_str_with_no_offet) {
  __arm_instruction result;
  arm_decode_instruction(&result, 0xE5810000); // STR R0,[R1]
  cr_assert_eq(result.type, INSTR_SINGLE_DATA_TRANSFER);
  cr_assert_eq(result.instr.single_data_transfer.add_offset_before_transfer, true);
  cr_assert_eq(result.instr.single_data_transfer.add_offset_to_base, true);
  cr_assert_eq(result.instr.single_data_transfer.transfer_byte, false);
  cr_assert_eq(result.instr.single_data_transfer.write_back_address, false);
  cr_assert_eq(result.instr.single_data_transfer.load, false);

  // The above instruction is assembled as the instr word followed by the value, so it loads
  // the value at offset 4 from PC (i.e. the word after the instruction we are decoding)
  cr_assert_eq(result.instr.single_data_transfer.base, REG_R1);
  cr_assert_eq(result.instr.single_data_transfer.source_dest, REG_R0);
  cr_assert_eq(result.instr.single_data_transfer.offset.is_immediate, true);
  cr_assert_eq(result.instr.single_data_transfer.offset.op.imm.value, 0x0);
}

Test(decoder, can_decode_an_str_with_immediate_offet) {
  __arm_instruction result;
  arm_decode_instruction(&result, 0xE4810038); // STR R0,[R1],#0x38
  cr_assert_eq(result.type, INSTR_SINGLE_DATA_TRANSFER);
  cr_assert_eq(result.instr.single_data_transfer.add_offset_before_transfer, false);
  cr_assert_eq(result.instr.single_data_transfer.add_offset_to_base, true);
  cr_assert_eq(result.instr.single_data_transfer.transfer_byte, false);
  cr_assert_eq(result.instr.single_data_transfer.write_back_address, false);
  cr_assert_eq(result.instr.single_data_transfer.load, false);

  // The above instruction is assembled as the instr word followed by the value, so it loads
  // the value at offset 4 from PC (i.e. the word after the instruction we are decoding)
  cr_assert_eq(result.instr.single_data_transfer.base, REG_R1);
  cr_assert_eq(result.instr.single_data_transfer.source_dest, REG_R0);
  cr_assert_eq(result.instr.single_data_transfer.offset.is_immediate, true);
  cr_assert_eq(result.instr.single_data_transfer.offset.op.imm.value, 0x38);
}

Test(decoder, can_decode_an_str_with_register_offet) {
  __arm_instruction result;
  arm_decode_instruction(&result, 0xE6810003); // STR R0,[R1],R3
  cr_assert_eq(result.type, INSTR_SINGLE_DATA_TRANSFER);
  cr_assert_eq(result.instr.single_data_transfer.add_offset_before_transfer, true);
  cr_assert_eq(result.instr.single_data_transfer.add_offset_to_base, true);
  cr_assert_eq(result.instr.single_data_transfer.transfer_byte, false);
  cr_assert_eq(result.instr.single_data_transfer.write_back_address, false);
  cr_assert_eq(result.instr.single_data_transfer.load, false);

  // The above instruction is assembled as the instr word followed by the value, so it loads
  // the value at offset 4 from PC (i.e. the word after the instruction we are decoding)
  cr_assert_eq(result.instr.single_data_transfer.base, REG_R1);
  cr_assert_eq(result.instr.single_data_transfer.source_dest, REG_R0);
  cr_assert_eq(result.instr.single_data_transfer.offset.is_immediate, false);
  cr_assert_eq(result.instr.single_data_transfer.offset.op.reg.reg, REG_R3);
  cr_assert_eq(result.instr.single_data_transfer.offset.op.reg.shift, 0);
}

