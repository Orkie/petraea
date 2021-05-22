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
} __arm_condition;

typedef enum {
  OPCODE_AND = 0b0000,//
  OPCODE_EOR = 0b0001,//
  OPCODE_SUB = 0b0010,//
  OPCODE_RSB = 0b0011,//
  OPCODE_ADD = 0b0100,//
  OPCODE_ADC = 0b0101,//
  OPCODE_SBC = 0b0110,//
  OPCODE_RSC = 0b0111,//
  OPCODE_TST = 0b1000,//
  OPCODE_TEQ = 0b1001,//
  OPCODE_CMP = 0b1010,
  OPCODE_CMN = 0b1011,
  OPCODE_ORR = 0b1100,
  OPCODE_MOV = 0b1101,
  OPCODE_BIC = 0b1110,
  OPCODE_MVN = 0b1111
} __arm_opcode;

typedef enum {
  INSTR_DATA_PROCESSING,
  INSTR_MULTIPLY,
  INSTR_MULTIPLY_LONG,
  INSTR_SINGLE_DATA_SWAP,
  INSTR_BRANCH_EXCHANGE,
  INSTR_HALFWORD_DATA_TRANSFER_REG_OFFSET,
  INSTR_HALFWORD_DATA_TRANSFER_IMM_OFFSET,
  INSTR_SINGLE_DATA_TRANSFER,
  INSTR_UNDEFINED,
  INSTR_BRANCH,
  // TODO - the rest
} __arm_instruction_type;

typedef struct {
  uint32_t value;
  bool carry;
  bool carryValid;
} __arm_immediate_operand;

typedef enum {
  SHIFT_LOGICAL_LEFT = 0b00,
  SHIFT_LOGICAL_RIGHT = 0b01,
  SHIFT_ARITHMETIC_RIGHT = 0b10,
  SHIFT_ROTATE_RIGHT = 0b11
} __arm_shift_type;

typedef struct {
  __arm_register reg;
  bool is_register_shift;
  __arm_shift_type shift_type;
  uint8_t shift_imm;
  __arm_register shift_reg;
} __arm_register_operand;

typedef struct {
  bool is_immediate;
  union Operand2 {
    __arm_immediate_operand imm;
    __arm_register_operand reg;
  } op;
} __arm_operand2;

///////////////////////////////////////////
// Data processing
///////////////////////////////////////////

typedef struct {
  __arm_opcode opcode;
  bool set_condition_codes;
  __arm_register operand1;
  __arm_operand2 operand2;
  __arm_register dest;
} __arm_instr_data_processing;

///////////////////////////////////////////
// Single data transfer
///////////////////////////////////////////

typedef struct {
  bool add_offset_before_transfer; // false = after
  bool add_offset; // false = subtract
  bool transfer_byte; // false = word
  bool unprivileged;
  bool write_back_address;
  bool load; // false == store
  __arm_register base;
  __arm_register source_dest;
  __arm_operand2 offset;
} __arm_instr_single_data_transfer;

///////////////////////////////////////////
// Branch exchange
///////////////////////////////////////////

typedef struct {
  __arm_register operand;
} __arm_instr_branch_exchange;

///////////////////////////////////////////
// Branch exchange
///////////////////////////////////////////

typedef struct {
  bool link;
  int32_t offset;
} __arm_instr_branch;

///////////////////////////////////////////

typedef union {
  __arm_instr_data_processing data_processing;
  __arm_instr_single_data_transfer single_data_transfer;
  __arm_instr_branch_exchange branch_exchange;
  __arm_instr_branch branch;
} __arm_instructions;

typedef struct {
  __arm_condition cond;
  __arm_instruction_type type;
  __arm_instructions instr;
} __arm_instruction;

extern int arm_decode_instruction(__arm_instruction* dest, uint32_t i);
extern int arm_execute_instruction(__arm_cpu* cpu, __arm_instruction* instr);
extern uint32_t eval_operand2(__arm_cpu* cpu, __arm_operand2* operand2, bool* carryValid, bool* carry);

#endif

