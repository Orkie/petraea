#include "instr.h"
#include "util.h"

static void arm_decode_operand2_immRot(__arm_operand2* dest, uint16_t op) {
  dest->is_immediate = true;
  unsigned int rotateBy = ((op&0xF00) >> 8) * 2;
  dest->op.imm.value = rrot32(op & 0xFF, rotateBy);
}

static void arm_decode_operand2_immVal(__arm_operand2* dest, uint16_t op) {
  dest->is_immediate = true;
  dest->op.imm.value = op & 0xFFF;
}

static void arm_decode_operand2_reg(__arm_operand2* dest, uint16_t op) {
  dest->is_immediate = false;
  dest->op.reg.reg = op & 0b1111;
  uint8_t shift = (op >> 4)&0xFF;
  dest->op.reg.is_register_shift = shift&0x1;
  dest->op.reg.shift_type = (shift >> 1) & 0b11;
  if(shift&0x1) {
    // register shift
    dest->op.reg.shift_reg = (shift >> 4) & 0b1111;
  } else {
    //immediate shift
    dest->op.reg.shift_imm = (shift >> 3) & 0b11111;
  }
}

static bool instr_is_data_processing(uint32_t i) {
  return (i & (0x3 << 26)) == 0x0;
}

static void decode_data_processing(__arm_instr_data_processing* dest, uint32_t i) {
  dest->opcode = (i >> 21) & 0xF;
  dest->set_condition_codes = (i >> 20) & 0x1;
  dest->operand1 = (i >> 16) & 0xF;
  dest->dest = (i >> 12) & 0xF;
  if((i >> 25) & 0x1) {
    arm_decode_operand2_immRot(&dest->operand2, (i&0xFFF));
  } else {
    arm_decode_operand2_reg(&dest->operand2, (i&0xFFF));
  }
}

static bool instr_is_single_data_transfer(uint32_t i) {
  return ((i&0x0E000000) == 0x04000000) // immediate offset
    || (((i&0x0E000000) == 0x06000000) && ((i&0x10) == 0x0));
}

static void decode_single_data_transfer(__arm_instr_single_data_transfer* dest, uint32_t i) {
  const bool p = (i >> 24) & 0x1;
  const bool w = (i >> 21) & 0x1;

  if(p == false) {
    dest->add_offset_before_transfer = false;
    dest->write_back_address = true;
  } else {
    dest->add_offset_before_transfer = true;
    dest->write_back_address = false;    
  }

  if(w == true) {
    if(p == false) {
      dest->unprivileged = true;
    } else {
      dest->write_back_address = true;
    }
  }
  
  dest->add_offset = (i >> 23) & 0x1;
  dest->transfer_byte = (i >> 22) & 0x1;
  dest->load = (i >> 20) & 0x1;
  dest->base = (i >> 16) & 0xF;
  dest->source_dest = (i >> 12) & 0xF;
  if((i >> 25) & 0x1) {
    arm_decode_operand2_reg(&dest->offset, (i&0xFFF));
  } else {
    arm_decode_operand2_immVal(&dest->offset, (i&0xFFF));
  }
}

static bool instr_is_undefined(uint32_t i) {
  return ((i&0x0E000000) == 0x06000000) && ((i&0x10) == 0x10);
}

static bool instr_is_branch_exchange(uint32_t i) {
  return (i&0x0FFFFFF0) == 0x12FFF10;
}

static bool instr_is_branch(uint32_t i) {
  return (i&0x0E000000) == 0x0A000000;
}

static void decode_branch(__arm_instr_branch* dest, uint32_t i) {
  dest->link = (i&0x01000000) == 0x01000000;
  uint32_t offsetRaw = (i&0x00FFFFFF);
  if(offsetRaw&0x00800000) {
    // negative
    dest->offset = (((i&0x00FFFFFF) << 2) | 0xFC000000)+8;
  } else {
    // positive
    dest->offset = ((i&0x00FFFFFF) << 2)+8;
  }
}

int arm_decode_instruction(__arm_instruction* dest, uint32_t i) {
  const unsigned int cond = (i&0xF0000000) >> 28;

  if(instr_is_branch_exchange(i)) {
    dest->type = INSTR_BRANCH_EXCHANGE;
    dest->instr.branch_exchange.operand = i&0xF;
  } else if(instr_is_branch(i)) {
    dest->type = INSTR_BRANCH;
    decode_branch(&dest->instr.branch, i);
  } else if(instr_is_data_processing(i)) {
    dest->type = INSTR_DATA_PROCESSING;
    decode_data_processing(&dest->instr.data_processing, i);
  } else if(instr_is_single_data_transfer(i)) {
    dest->type = INSTR_SINGLE_DATA_TRANSFER;
    decode_single_data_transfer(&dest->instr.single_data_transfer, i);
  } else if(instr_is_undefined(i)) {
    dest->type = INSTR_UNDEFINED;
  } else {
    return -1;
  }

  dest->cond = cond;
  return 0;
}

