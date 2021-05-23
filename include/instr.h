#ifndef pt_INSTR_Hpt_
#define pt_INSTR_Hpt_

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
  COND_AL = 0b1110,
  COND_NV = 0b1111
} pt_arm_condition;

typedef enum {
  OPCODE_AND = 0b0000,
  OPCODE_EOR = 0b0001,
  OPCODE_SUB = 0b0010,
  OPCODE_RSB = 0b0011,
  OPCODE_ADD = 0b0100,
  OPCODE_ADC = 0b0101,
  OPCODE_SBC = 0b0110,
  OPCODE_RSC = 0b0111,
  OPCODE_TST = 0b1000,
  OPCODE_TEQ = 0b1001,
  OPCODE_CMP = 0b1010,
  OPCODE_CMN = 0b1011,
  OPCODE_ORR = 0b1100,
  OPCODE_MOV = 0b1101,
  OPCODE_BIC = 0b1110,
  OPCODE_MVN = 0b1111
} pt_arm_opcode;

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
} pt_arm_instruction_type;

typedef struct {
  uint32_t value;
  bool carry;
  bool carryValid;
} pt_arm_immediate_operand;

typedef enum {
  SHIFT_LOGICAL_LEFT = 0b00,
  SHIFT_LOGICAL_RIGHT = 0b01,
  SHIFT_ARITHMETIC_RIGHT = 0b10,
  SHIFT_ROTATE_RIGHT = 0b11
} pt_arm_shift_type;

typedef struct {
  pt_arm_register reg;
  bool is_register_shift;
  pt_arm_shift_type shift_type;
  uint8_t shift_imm;
  pt_arm_register shift_reg;
} pt_arm_register_operand;

typedef struct {
  bool is_immediate;
  union Operand2 {
    pt_arm_immediate_operand imm;
    pt_arm_register_operand reg;
  } op;
} pt_arm_operand2;

///////////////////////////////////////////
// Data processing
///////////////////////////////////////////

typedef struct {
  pt_arm_opcode opcode;
  bool set_condition_codes;
  pt_arm_register operand1;
  pt_arm_operand2 operand2;
  pt_arm_register dest;
} pt_arm_instr_data_processing;

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
  pt_arm_register base;
  pt_arm_register source_dest;
  pt_arm_operand2 offset;
} pt_arm_instr_single_data_transfer;

///////////////////////////////////////////
// Branch exchange
///////////////////////////////////////////

typedef struct {
  pt_arm_register operand;
} pt_arm_instr_branch_exchange;

///////////////////////////////////////////
// Branch exchange
///////////////////////////////////////////

typedef struct {
  bool link;
  int32_t offset;
} pt_arm_instr_branch;

///////////////////////////////////////////

typedef union {
  pt_arm_instr_data_processing data_processing;
  pt_arm_instr_single_data_transfer single_data_transfer;
  pt_arm_instr_branch_exchange branch_exchange;
  pt_arm_instr_branch branch;
} pt_arm_instructions;

typedef struct {
  pt_arm_condition cond;
  pt_arm_instruction_type type;
  pt_arm_instructions instr;
} pt_arm_instruction;

extern int pt_arm_decode_instruction(pt_arm_instruction* dest, uint32_t i);
extern int pt_arm_execute_instruction(pt_arm_cpu* cpu, pt_arm_instruction* instr);
extern uint32_t _petraea_eval_operand2(pt_arm_cpu* cpu, pt_arm_operand2* operand2, bool* carryValid, bool* carry);
extern bool _petraea_eval_condition(pt_arm_cpu* cpu);

#endif

