#include "instr.h"
#include "util.h"

static void arm9_decode_operand2_imm(__arm9_operand2* dest, uint16_t op) {
  dest->is_immediate = true;
  unsigned int rotateBy = ((op&0xF00) >> 8) * 2;
  dest->op.imm.value = rrot32(op & 0xFF, rotateBy);
}

static void arm9_decode_operand2_reg(__arm9_operand2* dest, uint16_t op) {
  dest->is_immediate = false;
  dest->op.reg.reg = op & 0b1111;
  dest->op.reg.shift = op >> 4;
}

static bool instr_is_data_processing(uint32_t i) {
  return (i & (0x3 << 26)) == 0x0;
}

static void arm9_decode_data_processing(__arm9_instr_data_processing* dest, uint32_t i) {
  dest->opcode = (i >> 21) & 0xF;
  dest->set_condition_codes = (i >> 20) & 0x1;
  dest->operand1 = (i >> 16) & 0xF;
  dest->dest = (i >> 12) & 0xF;
  if((i & (0x1 << 25)) == 0x0) {
    arm9_decode_operand2_reg(&dest->operand2, (i&0xFFF));
  } else {
    arm9_decode_operand2_imm(&dest->operand2, (i&0xFFF));
  }
}

void arm9_decode_instruction(__arm9_instruction* dest, uint32_t i) {
  const unsigned int cond = (i&0xF0000000) >> 28;

  if(instr_is_data_processing(i)) {
    dest->type = INSTR_DATA_PROCESSING;
    arm9_decode_data_processing(&dest->instr.data_processing, i);
  } else {
    dest->type = INSTR_UNDEFINED;
  }

  dest->cond = cond;
}

