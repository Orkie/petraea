#ifndef __INSTR_H__
#define __INSTR_H__

#include <stdint.h>
#include <stdbool.h>
#include "cpu.h"

typedef enum {
  COND_EQ = 0b0000,
  COND_NE = 0b0001,
  COND_CS = 0b0010,
  COND_CC = 0b0011,
  COND_MI = 0b0100,
  COND_PL = 0b0101,
  COND_VS = 0b0110,
  COND_VC = 0b0111,
  COND_HI = 0b1000,
  COND_LS = 0b1001,
  COND_GE = 0b1010,
  COND_LT = 0b1011,
  COND_GT = 0b1100,
  COND_LE = 0b1101,
  COND_AL = 0b1110
} __arm9_condition;

typedef enum {
  OPERAND_AND = 0b0000,
  OPERAND_SUB = 0b0010,
  OPERAND_RSB = 0b0011,
  OPERAND_ADD = 0b0100,
  OPERAND_ADC = 0b0101,
  OPERAND_SBC = 0b0110,
  OPERAND_RSC = 0b0111,
  OPERAND_TST = 0b1000,
  OPERAND_TEQ = 0b1001,
  OPERAND_CMP = 0b1010,
  OPERAND_CMN = 0b1011,
  OPERAND_ORR = 0b1100,
  OPERAND_MOV = 0b1101,
  OPERAND_BIC = 0b1110,
  OPERAND_MVN = 0b1111
} __arm9_opcode;

typedef enum {
  INSTR_DATA_PROCESSING,
  INSTR_MULTIPLY,
  INSTR_MULTIPLY_LONG,
  INSTR_UNDEFINED
  // TODO - the rest
} __arm9_instruction_type;

typedef struct {
  uint32_t value;
} __arm9_immediate_operand;

typedef struct {
  __arm9_register reg;
  uint8_t shift;
} __arm9_register_operand;

typedef struct {
  bool is_immediate;
  union Operand2 {
    __arm9_immediate_operand imm;
    __arm9_register_operand reg;
  } op;
} __arm9_operand2;

typedef struct {
  __arm9_opcode opcode;
  bool set_condition_codes;
  __arm9_register operand1;
  __arm9_operand2 operand2;
  __arm9_register dest;
} __arm9_instr_data_processing;

typedef union {
  __arm9_instr_data_processing data_processing;
} __arm9_instructions;

typedef struct {
  __arm9_condition cond;
  __arm9_instruction_type type;
  __arm9_instructions instr;
} __arm9_instruction;

extern void arm9_decode_instruction(__arm9_instruction* dest, uint32_t i);

#endif

