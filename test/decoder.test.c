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
  cr_assert_eq(result.instr.data_processing.opcode, OPCODE_SUB);
  cr_assert_eq(result.instr.data_processing.dest, REG_R0);
  cr_assert_eq(result.instr.data_processing.set_condition_codes, false);
  cr_assert_eq(result.instr.data_processing.operand1, REG_R3);
}

Test(decoder, can_decode_a_data_processing_instr_updating_condition_flags) {
  __arm_instruction result;
  arm_decode_instruction(&result, 0xE0530001); // SUBS R0,R3,R1
  cr_assert_eq(result.type, INSTR_DATA_PROCESSING);
  cr_assert_eq(result.instr.data_processing.opcode, OPCODE_SUB);
  cr_assert_eq(result.instr.data_processing.dest, REG_R0);
  cr_assert_eq(result.instr.data_processing.set_condition_codes, true);
  cr_assert_eq(result.instr.data_processing.operand1, REG_R3);
}

Test(decoder, can_decode_a_data_processing_instr_with_operand2_immediate) {
  __arm_instruction result;
  arm_decode_instruction(&result, 0xE243002D); // SUB R0,R3,#45
  cr_assert_eq(result.type, INSTR_DATA_PROCESSING);
  cr_assert_eq(result.instr.data_processing.opcode, OPCODE_SUB);
  cr_assert_eq(result.instr.data_processing.dest, REG_R0);
  cr_assert_eq(result.instr.data_processing.operand1, REG_R3);
  cr_assert_eq(result.instr.data_processing.operand2.is_immediate, true);
  cr_assert_eq(result.instr.data_processing.operand2.op.imm.value, 45);
}

Test(decoder, can_decode_a_data_processing_instr_with_operand2_immediate_rotated_carry) {
  __arm_instruction result;
  arm_decode_instruction(&result, 0xE24304FF); // SUB R0,R3,#0xFF000000
  cr_assert_eq(result.type, INSTR_DATA_PROCESSING);
  cr_assert_eq(result.instr.data_processing.opcode, OPCODE_SUB);
  cr_assert_eq(result.instr.data_processing.dest, REG_R0);
  cr_assert_eq(result.instr.data_processing.operand1, REG_R3);
  cr_assert_eq(result.instr.data_processing.operand2.is_immediate, true);
  cr_assert_eq(result.instr.data_processing.operand2.op.imm.value, 0xFF000000);
  cr_assert_eq(result.instr.data_processing.operand2.op.imm.carryValid, true);
  cr_assert_eq(result.instr.data_processing.operand2.op.imm.carry, true);
}

Test(decoder, can_decode_a_data_processing_instr_with_operand2_immediate_rotated_no_carry) {
  __arm_instruction result;
  arm_decode_instruction(&result, 0xE243040F); // SUB R0,R3,#0x0F000000
  cr_assert_eq(result.type, INSTR_DATA_PROCESSING);
  cr_assert_eq(result.instr.data_processing.opcode, OPCODE_SUB);
  cr_assert_eq(result.instr.data_processing.dest, REG_R0);
  cr_assert_eq(result.instr.data_processing.operand1, REG_R3);
  cr_assert_eq(result.instr.data_processing.operand2.is_immediate, true);
  cr_assert_eq(result.instr.data_processing.operand2.op.imm.value, 0x0F000000);
  cr_assert_eq(result.instr.data_processing.operand2.op.imm.carryValid, true);
  cr_assert_eq(result.instr.data_processing.operand2.op.imm.carry, false);
}

Test(decoder, can_decode_a_data_processing_instr_with_operand2_immediate_rotated_0) {
  __arm_instruction result;
  arm_decode_instruction(&result, 0xE2430000); // SUB R0,R3,#0x0
  cr_assert_eq(result.type, INSTR_DATA_PROCESSING);
  cr_assert_eq(result.instr.data_processing.opcode, OPCODE_SUB);
  cr_assert_eq(result.instr.data_processing.dest, REG_R0);
  cr_assert_eq(result.instr.data_processing.operand1, REG_R3);
  cr_assert_eq(result.instr.data_processing.operand2.is_immediate, true);
  cr_assert_eq(result.instr.data_processing.operand2.op.imm.value, 0x0);
  cr_assert_eq(result.instr.data_processing.operand2.op.imm.carryValid, false);
  cr_assert_eq(result.instr.data_processing.operand2.op.imm.carry, false);
}

Test(decoder, can_decode_a_data_processing_instr_with_operand2_register) {
  __arm_instruction result;
  arm_decode_instruction(&result, 0xE0430004); // SUB R0,R3,R4
  cr_assert_eq(result.type, INSTR_DATA_PROCESSING);
  cr_assert_eq(result.instr.data_processing.opcode, OPCODE_SUB);
  cr_assert_eq(result.instr.data_processing.dest, REG_R0);
  cr_assert_eq(result.instr.data_processing.operand1, REG_R3);
  cr_assert_eq(result.instr.data_processing.operand2.is_immediate, false);
  cr_assert_eq(result.instr.data_processing.operand2.op.reg.reg, REG_R4);
  cr_assert_eq(result.instr.data_processing.operand2.op.reg.is_register_shift, false);
  cr_assert_eq(result.instr.data_processing.operand2.op.reg.shift_type, SHIFT_LOGICAL_LEFT);
  cr_assert_eq(result.instr.data_processing.operand2.op.reg.shift_imm, 0);
}

Test(decoder, can_decode_a_data_processing_instr_with_operand2_reg_shifted_by_immediate) {
  __arm_instruction result;
  arm_decode_instruction(&result, 0xE04101A2); // SUB R0,R1,R2,LSR #3
  cr_assert_eq(result.type, INSTR_DATA_PROCESSING);
  cr_assert_eq(result.instr.data_processing.opcode, OPCODE_SUB);
  cr_assert_eq(result.instr.data_processing.dest, REG_R0);
  cr_assert_eq(result.instr.data_processing.operand1, REG_R1);
  cr_assert_eq(result.instr.data_processing.operand2.is_immediate, false);
  cr_assert_eq(result.instr.data_processing.operand2.op.reg.reg, REG_R2);
  cr_assert_eq(result.instr.data_processing.operand2.op.reg.is_register_shift, false);
  cr_assert_eq(result.instr.data_processing.operand2.op.reg.shift_type, SHIFT_LOGICAL_RIGHT);
  cr_assert_eq(result.instr.data_processing.operand2.op.reg.shift_imm, 3);
}

Test(decoder, can_decode_a_data_processing_instr_with_operand2_reg_shifted_by_reg) {
  __arm_instruction result;
  arm_decode_instruction(&result, 0xE0410452); // SUB R0,R1,R2,ASR R4
  cr_assert_eq(result.type, INSTR_DATA_PROCESSING);
  cr_assert_eq(result.instr.data_processing.opcode, OPCODE_SUB);
  cr_assert_eq(result.instr.data_processing.dest, REG_R0);
  cr_assert_eq(result.instr.data_processing.operand1, REG_R1);
  cr_assert_eq(result.instr.data_processing.operand2.is_immediate, false);
  cr_assert_eq(result.instr.data_processing.operand2.op.reg.reg, REG_R2);
  cr_assert_eq(result.instr.data_processing.operand2.op.reg.is_register_shift, true);
  cr_assert_eq(result.instr.data_processing.operand2.op.reg.shift_type, SHIFT_ARITHMETIC_RIGHT);
  cr_assert_eq(result.instr.data_processing.operand2.op.reg.shift_reg, REG_R4);
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

Test(decoder, can_decode_an_ldr_with_immediate_offet) {
  __arm_instruction result;
  arm_decode_instruction(&result, 0xE59F0004); // LDR R0,[PC,#4]
  cr_assert_eq(result.type, INSTR_SINGLE_DATA_TRANSFER);
  cr_assert_eq(result.instr.single_data_transfer.add_offset_before_transfer, true);
  cr_assert_eq(result.instr.single_data_transfer.add_offset, true);
  cr_assert_eq(result.instr.single_data_transfer.transfer_byte, false);
  cr_assert_eq(result.instr.single_data_transfer.write_back_address, false);
  cr_assert_eq(result.instr.single_data_transfer.load, true);
  cr_assert_eq(result.instr.single_data_transfer.unprivileged, false);

  cr_assert_eq(result.instr.single_data_transfer.base, REG_PC);
  cr_assert_eq(result.instr.single_data_transfer.source_dest, REG_R0);
  cr_assert_eq(result.instr.single_data_transfer.offset.is_immediate, true);
  cr_assert_eq(result.instr.single_data_transfer.offset.op.imm.value, 4);
}

Test(decoder, can_decode_an_ldr_with_immediate_offet_subtract) {
  __arm_instruction result;
  arm_decode_instruction(&result, 0xE51F0004); // LDR R0,[PC,#-4]
  cr_assert_eq(result.type, INSTR_SINGLE_DATA_TRANSFER);
  cr_assert_eq(result.instr.single_data_transfer.add_offset_before_transfer, true);
  cr_assert_eq(result.instr.single_data_transfer.add_offset, false);
  cr_assert_eq(result.instr.single_data_transfer.transfer_byte, false);
  cr_assert_eq(result.instr.single_data_transfer.write_back_address, false);
  cr_assert_eq(result.instr.single_data_transfer.load, true);
  cr_assert_eq(result.instr.single_data_transfer.unprivileged, false);

  cr_assert_eq(result.instr.single_data_transfer.base, REG_PC);
  cr_assert_eq(result.instr.single_data_transfer.source_dest, REG_R0);
  cr_assert_eq(result.instr.single_data_transfer.offset.is_immediate, true);
  cr_assert_eq(result.instr.single_data_transfer.offset.op.imm.value, 4);
}

Test(decoder, can_decode_an_ldr_with_immediate_offet_forcing_user_mode) {
  __arm_instruction result;
  arm_decode_instruction(&result, 0xE4BF0000); // LDRT R0,[PC]
  cr_assert_eq(result.type, INSTR_SINGLE_DATA_TRANSFER);
  cr_assert_eq(result.instr.single_data_transfer.add_offset_before_transfer, false);
  cr_assert_eq(result.instr.single_data_transfer.add_offset, true);
  cr_assert_eq(result.instr.single_data_transfer.transfer_byte, false);
  cr_assert_eq(result.instr.single_data_transfer.write_back_address, true);
  cr_assert_eq(result.instr.single_data_transfer.load, true);
  cr_assert_eq(result.instr.single_data_transfer.unprivileged, true);

  cr_assert_eq(result.instr.single_data_transfer.base, REG_PC);
  cr_assert_eq(result.instr.single_data_transfer.source_dest, REG_R0);
  cr_assert_eq(result.instr.single_data_transfer.offset.is_immediate, true);
  cr_assert_eq(result.instr.single_data_transfer.offset.op.imm.value, 0);
}

Test(decoder, can_decode_an_ldr_with_immediate_offet_writeback) {
  __arm_instruction result;
  arm_decode_instruction(&result, 0xE5BF0004); // LDR R0,[PC,#4]!
  cr_assert_eq(result.type, INSTR_SINGLE_DATA_TRANSFER);
  cr_assert_eq(result.instr.single_data_transfer.add_offset_before_transfer, true);
  cr_assert_eq(result.instr.single_data_transfer.add_offset, true);
  cr_assert_eq(result.instr.single_data_transfer.transfer_byte, false);
  cr_assert_eq(result.instr.single_data_transfer.write_back_address, true);
  cr_assert_eq(result.instr.single_data_transfer.load, true);
  cr_assert_eq(result.instr.single_data_transfer.unprivileged, false);

  cr_assert_eq(result.instr.single_data_transfer.base, REG_PC);
  cr_assert_eq(result.instr.single_data_transfer.source_dest, REG_R0);
  cr_assert_eq(result.instr.single_data_transfer.offset.is_immediate, true);
  cr_assert_eq(result.instr.single_data_transfer.offset.op.imm.value, 4);
}

Test(decoder, can_decode_an_ldr_with_register_offset_post) {
  __arm_instruction result;
  arm_decode_instruction(&result, 0xE6910002); // LDR R0, [R1], R2
  cr_assert_eq(result.type, INSTR_SINGLE_DATA_TRANSFER);
  cr_assert_eq(result.instr.single_data_transfer.add_offset_before_transfer, false);
  cr_assert_eq(result.instr.single_data_transfer.add_offset, true);
  cr_assert_eq(result.instr.single_data_transfer.transfer_byte, false);
  cr_assert_eq(result.instr.single_data_transfer.write_back_address, true);
  cr_assert_eq(result.instr.single_data_transfer.load, true);
  cr_assert_eq(result.instr.single_data_transfer.unprivileged, false);

  cr_assert_eq(result.instr.single_data_transfer.base, REG_R1);
  cr_assert_eq(result.instr.single_data_transfer.source_dest, REG_R0);
  cr_assert_eq(result.instr.single_data_transfer.offset.op.reg.reg, REG_R2);
}

Test(decoder, can_decode_an_str_with_no_offet) {
  __arm_instruction result;
  arm_decode_instruction(&result, 0xE5810000); // STR R0,[R1]
  cr_assert_eq(result.type, INSTR_SINGLE_DATA_TRANSFER);
  cr_assert_eq(result.instr.single_data_transfer.add_offset_before_transfer, true);
  cr_assert_eq(result.instr.single_data_transfer.add_offset, true);
  cr_assert_eq(result.instr.single_data_transfer.transfer_byte, false);
  cr_assert_eq(result.instr.single_data_transfer.write_back_address, false);
  cr_assert_eq(result.instr.single_data_transfer.load, false);
  cr_assert_eq(result.instr.single_data_transfer.unprivileged, false);

  cr_assert_eq(result.instr.single_data_transfer.base, REG_R1);
  cr_assert_eq(result.instr.single_data_transfer.source_dest, REG_R0);
  cr_assert_eq(result.instr.single_data_transfer.offset.is_immediate, true);
  cr_assert_eq(result.instr.single_data_transfer.offset.op.imm.value, 0x0);
}

Test(decoder, can_decode_an_str_with_immediate_offet_post) {
  __arm_instruction result;
  arm_decode_instruction(&result, 0xE4810038); // STR R0,[R1],#0x38
  cr_assert_eq(result.type, INSTR_SINGLE_DATA_TRANSFER);
  cr_assert_eq(result.instr.single_data_transfer.add_offset_before_transfer, false);
  cr_assert_eq(result.instr.single_data_transfer.add_offset, true);
  cr_assert_eq(result.instr.single_data_transfer.transfer_byte, false);
  cr_assert_eq(result.instr.single_data_transfer.write_back_address, true);
  cr_assert_eq(result.instr.single_data_transfer.load, false);
  cr_assert_eq(result.instr.single_data_transfer.unprivileged, false);

  cr_assert_eq(result.instr.single_data_transfer.base, REG_R1);
  cr_assert_eq(result.instr.single_data_transfer.source_dest, REG_R0);
  cr_assert_eq(result.instr.single_data_transfer.offset.is_immediate, true);
  cr_assert_eq(result.instr.single_data_transfer.offset.op.imm.value, 0x38);
}

Test(decoder, can_decode_an_str_with_immediate_offet_pre_no_writeback) {
  __arm_instruction result;
  arm_decode_instruction(&result, 0xE5810038); // STR R0,[R1,#0x38]
  cr_assert_eq(result.type, INSTR_SINGLE_DATA_TRANSFER);
  cr_assert_eq(result.instr.single_data_transfer.add_offset_before_transfer, true);
  cr_assert_eq(result.instr.single_data_transfer.add_offset, true);
  cr_assert_eq(result.instr.single_data_transfer.transfer_byte, false);
  cr_assert_eq(result.instr.single_data_transfer.write_back_address, false);
  cr_assert_eq(result.instr.single_data_transfer.load, false);
  cr_assert_eq(result.instr.single_data_transfer.unprivileged, false);

  cr_assert_eq(result.instr.single_data_transfer.base, REG_R1);
  cr_assert_eq(result.instr.single_data_transfer.source_dest, REG_R0);
  cr_assert_eq(result.instr.single_data_transfer.offset.is_immediate, true);
  cr_assert_eq(result.instr.single_data_transfer.offset.op.imm.value, 0x38);
}

Test(decoder, can_decode_an_str_with_immediate_offet_pre_with_writeback) {
  __arm_instruction result;
  arm_decode_instruction(&result, 0xE5A10038); // STR R0,[R1,#0x38]!
  cr_assert_eq(result.type, INSTR_SINGLE_DATA_TRANSFER);
  cr_assert_eq(result.instr.single_data_transfer.add_offset_before_transfer, true);
  cr_assert_eq(result.instr.single_data_transfer.add_offset, true);
  cr_assert_eq(result.instr.single_data_transfer.transfer_byte, false);
  cr_assert_eq(result.instr.single_data_transfer.write_back_address, true);
  cr_assert_eq(result.instr.single_data_transfer.load, false);
  cr_assert_eq(result.instr.single_data_transfer.unprivileged, false);

  cr_assert_eq(result.instr.single_data_transfer.base, REG_R1);
  cr_assert_eq(result.instr.single_data_transfer.source_dest, REG_R0);
  cr_assert_eq(result.instr.single_data_transfer.offset.is_immediate, true);
  cr_assert_eq(result.instr.single_data_transfer.offset.op.imm.value, 0x38);
}

Test(decoder, can_decode_an_str_with_register_offet_post) {
  __arm_instruction result;
  arm_decode_instruction(&result, 0xE6810003); // STR R0,[R1],R3
  cr_assert_eq(result.type, INSTR_SINGLE_DATA_TRANSFER);
  cr_assert_eq(result.instr.single_data_transfer.add_offset_before_transfer, false);
  cr_assert_eq(result.instr.single_data_transfer.add_offset, true);
  cr_assert_eq(result.instr.single_data_transfer.transfer_byte, false);
  cr_assert_eq(result.instr.single_data_transfer.write_back_address, true);
  cr_assert_eq(result.instr.single_data_transfer.load, false);
  cr_assert_eq(result.instr.single_data_transfer.unprivileged, false);

  cr_assert_eq(result.instr.single_data_transfer.base, REG_R1);
  cr_assert_eq(result.instr.single_data_transfer.source_dest, REG_R0);
  cr_assert_eq(result.instr.single_data_transfer.offset.is_immediate, false);
  cr_assert_eq(result.instr.single_data_transfer.offset.op.reg.reg, REG_R3);
}

///////////////////////////////////////////
// Branch and exchange
///////////////////////////////////////////

Test(decoder, can_decode_bx) {
  __arm_instruction result;
  arm_decode_instruction(&result, 0xE12FFF14); // BX R4
  cr_assert_eq(result.type, INSTR_BRANCH_EXCHANGE);
  cr_assert_eq(result.instr.branch_exchange.operand, REG_R4);
}

///////////////////////////////////////////
// Branch
///////////////////////////////////////////

Test(decoder, can_decode_b) {
  __arm_instruction result;
  arm_decode_instruction(&result, 0xEA048D14); // B #0x123458
  cr_assert_eq(result.type, INSTR_BRANCH);
  cr_assert_eq(result.instr.branch.link, false);
  cr_assert_eq(result.instr.branch.offset, 0x123458);
}

Test(decoder, can_decode_b_with_negative_offset) {
  __arm_instruction result;
  arm_decode_instruction(&result, 0xEAFFFFFD); // B #-4
  cr_assert_eq(result.type, INSTR_BRANCH);
  cr_assert_eq(result.instr.branch.link, false);
  cr_assert_eq(result.instr.branch.offset, -4);
}

Test(decoder, can_decode_bl) {
  __arm_instruction result;
  arm_decode_instruction(&result, 0xEB00047E); // BL #0x1200
  cr_assert_eq(result.type, INSTR_BRANCH);
  cr_assert_eq(result.instr.branch.link, true);
  cr_assert_eq(result.instr.branch.offset, 0x1200);
}

