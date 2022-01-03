#include <stdlib.h>
#include <stdio.h>
#include <criterion/criterion.h>
#include "instr.h"

///////////////////////////////////////////
// Conditions
///////////////////////////////////////////

Test(decoder, can_decode_condition) {  
  pt_arm_instruction result;
  pt_arm_decode_instruction(&result, 0xc0800001); // ADDGT R0,R0,R1
  cr_assert_eq(result.cond, COND_GT);
}

///////////////////////////////////////////
// Data processing
///////////////////////////////////////////

Test(decoder, can_decode_a_data_processing_instr) {
  pt_arm_instruction result;
  pt_arm_decode_instruction(&result, 0xE0430001); // SUB R0,R3,R1
  cr_assert_eq(result.type, INSTR_DATA_PROCESSING);
  cr_assert_eq(result.instr.data_processing.opcode, OPCODE_SUB);
  cr_assert_eq(result.instr.data_processing.dest, REG_R0);
  cr_assert_eq(result.instr.data_processing.set_condition_codes, false);
  cr_assert_eq(result.instr.data_processing.operand1, REG_R3);
}

Test(decoder, can_decode_a_data_processing_instr_updating_condition_flags) {
  pt_arm_instruction result;
  pt_arm_decode_instruction(&result, 0xE0530001); // SUBS R0,R3,R1
  cr_assert_eq(result.type, INSTR_DATA_PROCESSING);
  cr_assert_eq(result.instr.data_processing.opcode, OPCODE_SUB);
  cr_assert_eq(result.instr.data_processing.dest, REG_R0);
  cr_assert_eq(result.instr.data_processing.set_condition_codes, true);
  cr_assert_eq(result.instr.data_processing.operand1, REG_R3);
}

Test(decoder, can_decode_a_data_processing_instr_with_operand2_immediate) {
  pt_arm_instruction result;
  pt_arm_decode_instruction(&result, 0xE243002D); // SUB R0,R3,#45
  cr_assert_eq(result.type, INSTR_DATA_PROCESSING);
  cr_assert_eq(result.instr.data_processing.opcode, OPCODE_SUB);
  cr_assert_eq(result.instr.data_processing.dest, REG_R0);
  cr_assert_eq(result.instr.data_processing.operand1, REG_R3);
  cr_assert_eq(result.instr.data_processing.operand2.is_immediate, true);
  cr_assert_eq(result.instr.data_processing.operand2.op.imm.value, 45);
}

Test(decoder, can_decode_a_data_processing_instr_with_operand2_immediate_rotated_carry) {
  pt_arm_instruction result;
  pt_arm_decode_instruction(&result, 0xE24304FF); // SUB R0,R3,#0xFF000000
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
  pt_arm_instruction result;
  pt_arm_decode_instruction(&result, 0xE243040F); // SUB R0,R3,#0x0F000000
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
  pt_arm_instruction result;
  pt_arm_decode_instruction(&result, 0xE2430000); // SUB R0,R3,#0x0
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
  pt_arm_instruction result;
  pt_arm_decode_instruction(&result, 0xE0430004); // SUB R0,R3,R4
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
  pt_arm_instruction result;
  pt_arm_decode_instruction(&result, 0xE04101A2); // SUB R0,R1,R2,LSR #3
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
  pt_arm_instruction result;
  pt_arm_decode_instruction(&result, 0xE0410452); // SUB R0,R1,R2,ASR R4
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
  pt_arm_instruction result;
  pt_arm_decode_instruction(&result, 0x46012516);
  cr_assert_eq(result.type, INSTR_UNDEFINED);
}

///////////////////////////////////////////
// Single data transfer
///////////////////////////////////////////

Test(decoder, can_decode_an_ldr_with_immediate_offet) {
  pt_arm_instruction result;
  pt_arm_decode_instruction(&result, 0xE59F0004); // LDR R0,[PC,#4]
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
  pt_arm_instruction result;
  pt_arm_decode_instruction(&result, 0xE51F0004); // LDR R0,[PC,#-4]
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
  pt_arm_instruction result;
  pt_arm_decode_instruction(&result, 0xE4BF0000); // LDRT R0,[PC]
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
  pt_arm_instruction result;
  pt_arm_decode_instruction(&result, 0xE5BF0004); // LDR R0,[PC,#4]!
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
  pt_arm_instruction result;
  pt_arm_decode_instruction(&result, 0xE6910002); // LDR R0, [R1], R2
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
  pt_arm_instruction result;
  pt_arm_decode_instruction(&result, 0xE5810000); // STR R0,[R1]
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
  pt_arm_instruction result;
  pt_arm_decode_instruction(&result, 0xE4810038); // STR R0,[R1],#0x38
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
  pt_arm_instruction result;
  pt_arm_decode_instruction(&result, 0xE5810038); // STR R0,[R1,#0x38]
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
  pt_arm_instruction result;
  pt_arm_decode_instruction(&result, 0xE5A10038); // STR R0,[R1,#0x38]!
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
  pt_arm_instruction result;
  pt_arm_decode_instruction(&result, 0xE6810003); // STR R0,[R1],R3
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
  pt_arm_instruction result;
  pt_arm_decode_instruction(&result, 0xE12FFF14); // BX R4
  cr_assert_eq(result.type, INSTR_BRANCH_EXCHANGE);
  cr_assert_eq(result.instr.branch_exchange.operand, REG_R4);
}

///////////////////////////////////////////
// Branch
///////////////////////////////////////////

Test(decoder, can_decode_b) {
  pt_arm_instruction result;
  pt_arm_decode_instruction(&result, 0xEA048D14); // B #0x123458
  cr_assert_eq(result.type, INSTR_BRANCH);
  cr_assert_eq(result.instr.branch.link, false);
  cr_assert_eq(result.instr.branch.offset, 0x123458);
}

Test(decoder, can_decode_b_with_negative_offset) {
  pt_arm_instruction result;
  pt_arm_decode_instruction(&result, 0xEAFFFFFD); // B #-4
  cr_assert_eq(result.type, INSTR_BRANCH);
  cr_assert_eq(result.instr.branch.link, false);
  cr_assert_eq(result.instr.branch.offset, -4);
}

Test(decoder, can_decode_bl) {
  pt_arm_instruction result;
  pt_arm_decode_instruction(&result, 0xEB00047E); // BL #0x1200
  cr_assert_eq(result.type, INSTR_BRANCH);
  cr_assert_eq(result.instr.branch.link, true);
  cr_assert_eq(result.instr.branch.offset, 0x1200);
}

///////////////////////////////////////////
// Halfword data transfer
///////////////////////////////////////////

Test(decoder, can_decode_ldrh_immediate_offset) {
  pt_arm_instruction result;
  pt_arm_decode_instruction(&result, 0xE1D101B8); // LDRH R0, [R1, #24]
  cr_assert_eq(result.type, INSTR_HALFWORD_DATA_TRANSFER);
  cr_assert_eq(result.instr.halfword_data_transfer.add_offset_before_transfer, true);
  cr_assert_eq(result.instr.halfword_data_transfer.add_offset, true);
  cr_assert_eq(result.instr.halfword_data_transfer.write_back_address, false);
  cr_assert_eq(result.instr.halfword_data_transfer.load, true);
  cr_assert_eq(result.instr.halfword_data_transfer.base, REG_R1);
  cr_assert_eq(result.instr.halfword_data_transfer.source_dest, REG_R0);
  cr_assert_eq(result.instr.halfword_data_transfer.is_immediate_offset, true);
  cr_assert_eq(result.instr.halfword_data_transfer.offset_imm, 24);
  cr_assert_eq(result.instr.halfword_data_transfer.is_signed, false);
  cr_assert_eq(result.instr.halfword_data_transfer.transfer_byte, false);
}

Test(decoder, can_decode_ldrh_reg_offset) {
  pt_arm_instruction result;
  pt_arm_decode_instruction(&result, 0xE19100B2); // STRH R0, [R1], R2
  cr_assert_eq(result.type, INSTR_HALFWORD_DATA_TRANSFER);
  cr_assert_eq(result.instr.halfword_data_transfer.add_offset_before_transfer, true);
  cr_assert_eq(result.instr.halfword_data_transfer.add_offset, true);
  cr_assert_eq(result.instr.halfword_data_transfer.write_back_address, false);
  cr_assert_eq(result.instr.halfword_data_transfer.load, true);
  cr_assert_eq(result.instr.halfword_data_transfer.base, REG_R1);
  cr_assert_eq(result.instr.halfword_data_transfer.source_dest, REG_R0);
  cr_assert_eq(result.instr.halfword_data_transfer.is_immediate_offset, false);
  cr_assert_eq(result.instr.halfword_data_transfer.offset_reg, REG_R2);
  cr_assert_eq(result.instr.halfword_data_transfer.is_signed, false);
  cr_assert_eq(result.instr.halfword_data_transfer.transfer_byte, false);
}

Test(decoder, can_decode_ldrh_post_increment) {
  pt_arm_instruction result;
  pt_arm_decode_instruction(&result, 0xE0D101B8); // LDRH R0, [R1], #24
  cr_assert_eq(result.type, INSTR_HALFWORD_DATA_TRANSFER);
  cr_assert_eq(result.instr.halfword_data_transfer.add_offset_before_transfer, false);
  cr_assert_eq(result.instr.halfword_data_transfer.add_offset, true);
  cr_assert_eq(result.instr.halfword_data_transfer.write_back_address, true);
  cr_assert_eq(result.instr.halfword_data_transfer.load, true);
  cr_assert_eq(result.instr.halfword_data_transfer.base, REG_R1);
  cr_assert_eq(result.instr.halfword_data_transfer.source_dest, REG_R0);
  cr_assert_eq(result.instr.halfword_data_transfer.is_immediate_offset, true);
  cr_assert_eq(result.instr.halfword_data_transfer.offset_imm, 24);
  cr_assert_eq(result.instr.halfword_data_transfer.is_signed, false);
  cr_assert_eq(result.instr.halfword_data_transfer.transfer_byte, false);
}

Test(decoder, can_decode_ldrh_pre_increment_write_back) {
  pt_arm_instruction result;
  pt_arm_decode_instruction(&result, 0xE1F101B8); // LDRH R0, [R1, #24]!
  cr_assert_eq(result.type, INSTR_HALFWORD_DATA_TRANSFER);
  cr_assert_eq(result.instr.halfword_data_transfer.add_offset_before_transfer, true);
  cr_assert_eq(result.instr.halfword_data_transfer.add_offset, true);
  cr_assert_eq(result.instr.halfword_data_transfer.write_back_address, true);
  cr_assert_eq(result.instr.halfword_data_transfer.load, true);
  cr_assert_eq(result.instr.halfword_data_transfer.base, REG_R1);
  cr_assert_eq(result.instr.halfword_data_transfer.source_dest, REG_R0);
  cr_assert_eq(result.instr.halfword_data_transfer.is_immediate_offset, true);
  cr_assert_eq(result.instr.halfword_data_transfer.offset_imm, 24);
  cr_assert_eq(result.instr.halfword_data_transfer.is_signed, false);
  cr_assert_eq(result.instr.halfword_data_transfer.transfer_byte, false);
}

Test(decoder, can_decode_ldrsb) {
  pt_arm_instruction result;
  pt_arm_decode_instruction(&result, 0xE1D101D8); // LDRSB R0, [R1, #24]
  cr_assert_eq(result.type, INSTR_HALFWORD_DATA_TRANSFER);
  cr_assert_eq(result.instr.halfword_data_transfer.add_offset_before_transfer, true);
  cr_assert_eq(result.instr.halfword_data_transfer.add_offset, true);
  cr_assert_eq(result.instr.halfword_data_transfer.write_back_address, false);
  cr_assert_eq(result.instr.halfword_data_transfer.load, true);
  cr_assert_eq(result.instr.halfword_data_transfer.base, REG_R1);
  cr_assert_eq(result.instr.halfword_data_transfer.source_dest, REG_R0);
  cr_assert_eq(result.instr.halfword_data_transfer.is_immediate_offset, true);
  cr_assert_eq(result.instr.halfword_data_transfer.offset_imm, 24);
  cr_assert_eq(result.instr.halfword_data_transfer.is_signed, true);
  cr_assert_eq(result.instr.halfword_data_transfer.transfer_byte, true);
}

Test(decoder, can_decode_ldrsh) {
  pt_arm_instruction result;
  pt_arm_decode_instruction(&result, 0xE1D101F8); // LDRSH R0, [R1, #24]
  cr_assert_eq(result.type, INSTR_HALFWORD_DATA_TRANSFER);
  cr_assert_eq(result.instr.halfword_data_transfer.add_offset_before_transfer, true);
  cr_assert_eq(result.instr.halfword_data_transfer.add_offset, true);
  cr_assert_eq(result.instr.halfword_data_transfer.write_back_address, false);
  cr_assert_eq(result.instr.halfword_data_transfer.load, true);
  cr_assert_eq(result.instr.halfword_data_transfer.base, REG_R1);
  cr_assert_eq(result.instr.halfword_data_transfer.source_dest, REG_R0);
  cr_assert_eq(result.instr.halfword_data_transfer.is_immediate_offset, true);
  cr_assert_eq(result.instr.halfword_data_transfer.offset_imm, 24);
  cr_assert_eq(result.instr.halfword_data_transfer.is_signed, true);
  cr_assert_eq(result.instr.halfword_data_transfer.transfer_byte, false);
}

Test(decoder, can_decode_ldrh_subtract_offset) {
  pt_arm_instruction result;
  pt_arm_decode_instruction(&result, 0xE15101B8); // LDRH R0, [R1, #-24]
  cr_assert_eq(result.type, INSTR_HALFWORD_DATA_TRANSFER);
  cr_assert_eq(result.instr.halfword_data_transfer.add_offset_before_transfer, true);
  cr_assert_eq(result.instr.halfword_data_transfer.add_offset, false);
  cr_assert_eq(result.instr.halfword_data_transfer.write_back_address, false);
  cr_assert_eq(result.instr.halfword_data_transfer.load, true);
  cr_assert_eq(result.instr.halfword_data_transfer.base, REG_R1);
  cr_assert_eq(result.instr.halfword_data_transfer.source_dest, REG_R0);
  cr_assert_eq(result.instr.halfword_data_transfer.is_immediate_offset, true);
  cr_assert_eq(result.instr.halfword_data_transfer.offset_imm, 24);
  cr_assert_eq(result.instr.halfword_data_transfer.is_signed, false);
  cr_assert_eq(result.instr.halfword_data_transfer.transfer_byte, false);
}

Test(decoder, can_decode_strh_immediate_offset) {
  pt_arm_instruction result;
  pt_arm_decode_instruction(&result, 0xE1C101B8); // STRH R0, [R1, #24]
  cr_assert_eq(result.type, INSTR_HALFWORD_DATA_TRANSFER);
  cr_assert_eq(result.instr.halfword_data_transfer.add_offset_before_transfer, true);
  cr_assert_eq(result.instr.halfword_data_transfer.add_offset, true);
  cr_assert_eq(result.instr.halfword_data_transfer.write_back_address, false);
  cr_assert_eq(result.instr.halfword_data_transfer.load, false);
  cr_assert_eq(result.instr.halfword_data_transfer.base, REG_R1);
  cr_assert_eq(result.instr.halfword_data_transfer.source_dest, REG_R0);
  cr_assert_eq(result.instr.halfword_data_transfer.is_immediate_offset, true);
  cr_assert_eq(result.instr.halfword_data_transfer.offset_imm, 24);
  cr_assert_eq(result.instr.halfword_data_transfer.is_signed, false);
  cr_assert_eq(result.instr.halfword_data_transfer.transfer_byte, false);
}

///////////////////////////////////////////
// Swap
///////////////////////////////////////////

Test(decoder, can_decode_swap) {
  pt_arm_instruction result;
  pt_arm_decode_instruction(&result, 0xE1001092); // SWP R1, R2, [R0]
  cr_assert_eq(result.type, INSTR_SWAP);
  cr_assert_eq(result.instr.swap.transfer_byte, false);
  cr_assert_eq(result.instr.swap.value, REG_R2);
  cr_assert_eq(result.instr.swap.dest, REG_R1);
  cr_assert_eq(result.instr.swap.addr, REG_R0);
}

Test(decoder, can_decode_swap_byte) {
  pt_arm_instruction result;
  pt_arm_decode_instruction(&result, 0xE1401092); // SWPB R1, R2, [R0]
  cr_assert_eq(result.type, INSTR_SWAP);
  cr_assert_eq(result.instr.swap.transfer_byte, true);
  cr_assert_eq(result.instr.swap.value, REG_R2);
  cr_assert_eq(result.instr.swap.dest, REG_R1);
  cr_assert_eq(result.instr.swap.addr, REG_R0);
}

///////////////////////////////////////////
// Block data transfer
///////////////////////////////////////////

Test(decoder, can_decode_ldmia) {
  pt_arm_instruction result;
  pt_arm_decode_instruction(&result, 0xE8930101); // ldmia r3,{r0,r8}

  cr_assert_eq(result.type, INSTR_BLOCK_DATA_TRANSFER);
  cr_assert_eq(result.instr.block_data_transfer.add_offset_before_transfer, false);
  cr_assert_eq(result.instr.block_data_transfer.add_offset, true);
  cr_assert_eq(result.instr.block_data_transfer.load_psr_or_force_user_mode, false);
  cr_assert_eq(result.instr.block_data_transfer.write_back_address, false);
  cr_assert_eq(result.instr.block_data_transfer.load, true);
  cr_assert_eq(result.instr.block_data_transfer.register_list, 0x101);
  cr_assert_eq(result.instr.block_data_transfer.base, REG_R3);
}

Test(decoder, can_decode_ldmib) {
  pt_arm_instruction result;
  pt_arm_decode_instruction(&result, 0xE9930101); // ldmib r3,{r0,r8}

  cr_assert_eq(result.type, INSTR_BLOCK_DATA_TRANSFER);
  cr_assert_eq(result.instr.block_data_transfer.add_offset_before_transfer, true);
  cr_assert_eq(result.instr.block_data_transfer.add_offset, true);
  cr_assert_eq(result.instr.block_data_transfer.load_psr_or_force_user_mode, false);
  cr_assert_eq(result.instr.block_data_transfer.write_back_address, false);
  cr_assert_eq(result.instr.block_data_transfer.load, true);
  cr_assert_eq(result.instr.block_data_transfer.register_list, 0x101);
  cr_assert_eq(result.instr.block_data_transfer.base, REG_R3);
}

Test(decoder, can_decode_ldmda) {
  pt_arm_instruction result;
  pt_arm_decode_instruction(&result, 0xE8130101); // ldmda r3,{r0,r8}

  cr_assert_eq(result.type, INSTR_BLOCK_DATA_TRANSFER);
  cr_assert_eq(result.instr.block_data_transfer.add_offset_before_transfer, false);
  cr_assert_eq(result.instr.block_data_transfer.add_offset, false);
  cr_assert_eq(result.instr.block_data_transfer.load_psr_or_force_user_mode, false);
  cr_assert_eq(result.instr.block_data_transfer.write_back_address, false);
  cr_assert_eq(result.instr.block_data_transfer.load, true);
  cr_assert_eq(result.instr.block_data_transfer.register_list, 0x101);
  cr_assert_eq(result.instr.block_data_transfer.base, REG_R3);
}

Test(decoder, can_decode_ldmdb) {
  pt_arm_instruction result;
  pt_arm_decode_instruction(&result, 0xE9130101); // ldmdb r3,{r0,r8}

  cr_assert_eq(result.type, INSTR_BLOCK_DATA_TRANSFER);
  cr_assert_eq(result.instr.block_data_transfer.add_offset_before_transfer, true);
  cr_assert_eq(result.instr.block_data_transfer.add_offset, false);
  cr_assert_eq(result.instr.block_data_transfer.load_psr_or_force_user_mode, false);
  cr_assert_eq(result.instr.block_data_transfer.write_back_address, false);
  cr_assert_eq(result.instr.block_data_transfer.load, true);
  cr_assert_eq(result.instr.block_data_transfer.register_list, 0x101);
  cr_assert_eq(result.instr.block_data_transfer.base, REG_R3);
}

Test(decoder, can_decode_ldmdb_write_back) {
  pt_arm_instruction result;
  pt_arm_decode_instruction(&result, 0xE9330101); // ldmdb r3!,{r0,r8}

  cr_assert_eq(result.type, INSTR_BLOCK_DATA_TRANSFER);
  cr_assert_eq(result.instr.block_data_transfer.add_offset_before_transfer, true);
  cr_assert_eq(result.instr.block_data_transfer.add_offset, false);
  cr_assert_eq(result.instr.block_data_transfer.load_psr_or_force_user_mode, false);
  cr_assert_eq(result.instr.block_data_transfer.write_back_address, true);
  cr_assert_eq(result.instr.block_data_transfer.load, true);
  cr_assert_eq(result.instr.block_data_transfer.register_list, 0x101);
  cr_assert_eq(result.instr.block_data_transfer.base, REG_R3);
}

Test(decoder, can_decode_ldmdb_spr) {
  pt_arm_instruction result;
  pt_arm_decode_instruction(&result, 0xE9530101); // ldmdb r3,{r0,r8}^

  cr_assert_eq(result.type, INSTR_BLOCK_DATA_TRANSFER);
  cr_assert_eq(result.instr.block_data_transfer.add_offset_before_transfer, true);
  cr_assert_eq(result.instr.block_data_transfer.add_offset, false);
  cr_assert_eq(result.instr.block_data_transfer.load_psr_or_force_user_mode, true);
  cr_assert_eq(result.instr.block_data_transfer.write_back_address, false);
  cr_assert_eq(result.instr.block_data_transfer.load, true);
  cr_assert_eq(result.instr.block_data_transfer.register_list, 0x101);
  cr_assert_eq(result.instr.block_data_transfer.base, REG_R3);
}

Test(decoder, can_decode_stmia) {
  pt_arm_instruction result;
  pt_arm_decode_instruction(&result, 0xE8830101); // stmia r3,{r0,r8}

  cr_assert_eq(result.type, INSTR_BLOCK_DATA_TRANSFER);
  cr_assert_eq(result.instr.block_data_transfer.add_offset_before_transfer, false);
  cr_assert_eq(result.instr.block_data_transfer.add_offset, true);
  cr_assert_eq(result.instr.block_data_transfer.load_psr_or_force_user_mode, false);
  cr_assert_eq(result.instr.block_data_transfer.write_back_address, false);
  cr_assert_eq(result.instr.block_data_transfer.load, false);
  cr_assert_eq(result.instr.block_data_transfer.register_list, 0x101);
  cr_assert_eq(result.instr.block_data_transfer.base, REG_R3);
}

///////////////////////////////////////////
// Co-processor access
///////////////////////////////////////////

Test(decoder, can_decode_mrc) {
  pt_arm_instruction result;
  pt_arm_decode_instruction(&result, 0xEE123FB4); // mrc p15, 0, r3, c2, c4, 5

  cr_assert_eq(result.type, INSTR_COPROCESSOR_REGISTER_TRANSFER);
  cr_assert_eq(result.instr.coprocessor_register_transfer.load, true);
  cr_assert_eq(result.instr.coprocessor_register_transfer.source_dest, REG_R3);
  cr_assert_eq(result.instr.coprocessor_register_transfer.cp_reg, 2);
  cr_assert_eq(result.instr.coprocessor_register_transfer.cp_num, 15);
  cr_assert_eq(result.instr.coprocessor_register_transfer.crm, 4);
  cr_assert_eq(result.instr.coprocessor_register_transfer.opcode_2, 5);
}

Test(decoder, can_decode_mcr) {
  pt_arm_instruction result;
  pt_arm_decode_instruction(&result, 0xEE018DD2); // mcr p13, 0, r8, c1, c2, 6

  cr_assert_eq(result.type, INSTR_COPROCESSOR_REGISTER_TRANSFER);
  cr_assert_eq(result.instr.coprocessor_register_transfer.load, false);
  cr_assert_eq(result.instr.coprocessor_register_transfer.source_dest, REG_R8);
  cr_assert_eq(result.instr.coprocessor_register_transfer.cp_reg, 1);
  cr_assert_eq(result.instr.coprocessor_register_transfer.cp_num, 13);
  cr_assert_eq(result.instr.coprocessor_register_transfer.crm, 2);
  cr_assert_eq(result.instr.coprocessor_register_transfer.opcode_2, 6);
}


