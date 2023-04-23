#include "instr.h"
#include "util.h"

static inline __attribute__((always_inline)) void arm_decode_operand2_immRot(pt_arm_operand2* dest, uint16_t op) {
  dest->is_immediate = true;
  unsigned int rotateBy = ((op&0xF00) >> 8) * 2;
  dest->op.imm.value = rrot32(op & 0xFF, rotateBy);
  dest->op.imm.carryValid = rotateBy != 0;
  dest->op.imm.carry = (dest->op.imm.value >> 31)&0x1;
}

static inline __attribute__((always_inline)) void arm_decode_operand2_immVal(pt_arm_operand2* dest, uint16_t op) {
  dest->is_immediate = true;
  dest->op.imm.value = op & 0xFFF;
}

static inline __attribute__((always_inline)) void arm_decode_operand2_reg(pt_arm_operand2* dest, uint16_t op) {
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

static inline __attribute__((always_inline)) bool instr_is_data_processing(uint32_t i) {
  return (i & (0x3 << 26)) == 0x0;
}

static inline __attribute__((always_inline)) void decode_data_processing(pt_arm_instr_data_processing* dest, uint32_t i) {
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

static inline __attribute__((always_inline)) bool instr_is_single_data_transfer(uint32_t i) {
  return ((i&0x0E000000) == 0x04000000) // immediate offset
    || (((i&0x0E000000) == 0x06000000) && ((i&0x10) == 0x0));
}

static inline __attribute__((always_inline)) void decode_single_data_transfer(pt_arm_instr_single_data_transfer* dest, uint32_t i) {
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

static inline __attribute__((always_inline)) bool instr_is_undefined(uint32_t i) {
  return ((i&0x0E000000) == 0x06000000) && ((i&0x10) == 0x10);
}

static inline __attribute__((always_inline)) bool instr_is_branch_exchange(uint32_t i) {
  return (i&0x0FFFFFF0) == 0x12FFF10;
}

static inline __attribute__((always_inline)) bool instr_is_branch(uint32_t i) {
  return (i&0x0E000000) == 0x0A000000;
}

static inline __attribute__((always_inline)) void decode_branch(pt_arm_instr_branch* dest, uint32_t i) {
  dest->link = (i&0x01000000) == 0x01000000;
  uint32_t offsetRaw = (i&0x00FFFFFF);
  if(offsetRaw&0x00800000) {
    // negative
    dest->offset = ((i&0x00FFFFFF) << 2) | 0xFC000000;
  } else {
    // positive
    dest->offset = (i&0x00FFFFFF) << 2;
  }
}

static inline __attribute__((always_inline)) bool instr_is_halfword_data_transfer(uint32_t i) {
  return (((i>>25)&0x7) == 0x0) && (((i>>4)&0xF) == 0xD || ((i>>4)&0xF) == 0xB || ((i>>4)&0xF) == 0xF);
}

static inline __attribute__((always_inline)) void decode_halfword_data_transfer(pt_arm_instr_halfword_data_transfer* dest, uint32_t i) {
  const bool p = (i >> 24) & 0x1;
  const bool w = (i >> 21) & 0x1;

  if(p == false) {
    dest->add_offset_before_transfer = false;
    dest->write_back_address = true;
  } else {
    dest->add_offset_before_transfer = true;
    dest->write_back_address = false;    
  }

  if(w == true && p == true) {
    dest->write_back_address = true;
  }

  dest->add_offset = (i >> 23) & 0x1;
  dest->load = (i >> 20) & 0x1;
  dest->base = (i >> 16) & 0xF;
  dest->source_dest = (i >> 12) & 0xF;
  dest->is_signed = (i >> 6) & 0x1;
  dest->transfer_byte = !((i >> 5) & 0x1);

  if((i >> 22) & 0x1) {
    dest->is_immediate_offset = true;
    dest->offset_imm = ((i >> 4) & 0xF0) | (i&0xF);
  } else {
    dest->is_immediate_offset = false;
    dest->offset_reg = (i&0xF);
  }
}

static inline __attribute__((always_inline)) bool instr_is_swap(uint32_t i) {
  return (i&0xFC000F0) == 0x1000090 || (i&0xFC000F0) == 0x1400090;
}

static inline __attribute__((always_inline)) void decode_swap(pt_arm_instr_swap* dest, uint32_t i) {
  dest->transfer_byte = (i >> 22) & 0x1;
  dest->value = i&0xF;
  dest->dest = (i>>12)&0xF;
  dest->addr = (i>>16)&0xF;
}

static inline __attribute__((always_inline)) bool instr_is_block_data_transfer(uint32_t i) {
  return ((i&0x0E000000) == 0x08000000);
}

static inline __attribute__((always_inline)) void decode_block_data_transfer(pt_arm_instr_block_data_transfer* dest, uint32_t i) {
  dest->add_offset_before_transfer = (i>>24)&0x1;
  dest->add_offset = (i>>23)&0x1;
  dest->load_psr_or_force_user_mode = (i>>22)&0x1;
  dest->write_back_address = (i>>21)&0x1;
  dest->load = (i>>20)&0x1;
  dest->base = (i>>16)&0xF;
  dest->register_list = i&0xFFFF;
}

static inline __attribute__((always_inline)) bool instr_is_coprocessor_register_transfer(uint32_t i) {
  return ((i&0x0F000010) == 0x0E000010);
}

static inline __attribute__((always_inline)) void decode_coprocessor_register_transfer(pt_arm_instr_coprocessor_register_transfer* dest, uint32_t i) {
  dest->crm = i&0xF;
  dest->opcode_2 = (i>>5)&0x7;
  dest->cp_num = (i>>8)&0xF;
  dest->source_dest = (i>>12)&0xF;
  dest->cp_reg = (i>>16)&0xF;
  dest->load = (i>>20)&0x1;
}

static inline __attribute__((always_inline)) bool instr_is_multiply(uint32_t i) {
  return ((i&0x0FC000F0) == 0x90);
}

static inline __attribute__((always_inline)) void decode_multiply(pt_arm_instr_multiply* dest, uint32_t i) {
  dest->rd = (i>>16)&0xF;
  dest->rm = i&0xF;
  dest->rs = (i>>8)&0xF;
  dest->rn = (i>>12)&0xF;
  dest->set_condition_codes = (i>>20)&0x1;
  dest->accumulate = (i>>21)&0x1;
}

static inline __attribute__((always_inline)) bool instr_is_multiply_long(uint32_t i) {
  return ((i&0x0F8000F0) == 0x800090);
}

static inline __attribute__((always_inline)) void decode_multiply_long(pt_arm_instr_multiply_long* dest, uint32_t i) {
  dest->rd_hi = (i>>16)&0xF;
  dest->rd_lo = (i>>12)&0xF;
  dest->rs = (i>>8)&0xF;
  dest->rm = i&0xF;
  dest->set_condition_codes = (i>>20)&0x1;
  dest->accumulate = (i>>21)&0x1;
  dest->is_signed = (i>>22)&0x1;
}

int inline __attribute__((always_inline)) pt_arm_decode_instruction(pt_arm_instruction* dest, uint32_t i) {
  const unsigned int cond = (i&0xF0000000) >> 28;

  if(instr_is_single_data_transfer(i)) {
    dest->type = INSTR_SINGLE_DATA_TRANSFER;
    decode_single_data_transfer(&dest->instr.single_data_transfer, i);
  } else if(instr_is_branch_exchange(i)) {
    dest->type = INSTR_BRANCH_EXCHANGE;
    dest->instr.branch_exchange.operand = i&0xF;
  } else if(instr_is_branch(i)) {
    dest->type = INSTR_BRANCH;
    decode_branch(&dest->instr.branch, i);
  } else if(instr_is_halfword_data_transfer(i)) {
    dest->type = INSTR_HALFWORD_DATA_TRANSFER;
    decode_halfword_data_transfer(&dest->instr.halfword_data_transfer, i);
  } else if(instr_is_swap(i)) {
    dest->type = INSTR_SWAP;
    decode_swap(&dest->instr.swap, i);
  } else if(instr_is_multiply(i)) {
    dest->type = INSTR_MULTIPLY;
    decode_multiply(&dest->instr.multiply, i);
  } else if(instr_is_multiply_long(i)) {
    dest->type = INSTR_MULTIPLY_LONG;
    decode_multiply_long(&dest->instr.multiply_long, i);
  } else if(instr_is_data_processing(i)) {
    dest->type = INSTR_DATA_PROCESSING;
    decode_data_processing(&dest->instr.data_processing, i);
  } else if(instr_is_block_data_transfer(i)) {
    dest->type = INSTR_BLOCK_DATA_TRANSFER;
    decode_block_data_transfer(&dest->instr.block_data_transfer, i);
  } else if(instr_is_coprocessor_register_transfer(i)) {
    dest->type = INSTR_COPROCESSOR_REGISTER_TRANSFER;
    decode_coprocessor_register_transfer(&dest->instr.coprocessor_register_transfer, i);
  } else if(instr_is_undefined(i)) {
    dest->type = INSTR_UNDEFINED;
  } else {
    return -1;
  }

  dest->cond = cond;
  return 0;
}

