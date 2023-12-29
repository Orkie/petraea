#ifndef _INSTR_H_
#define _INSTR_H_

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
  INSTR_BRANCH_EXCHANGE,
  INSTR_HALFWORD_DATA_TRANSFER,
  INSTR_SINGLE_DATA_TRANSFER,
  INSTR_SWAP,
  INSTR_UNDEFINED,
  INSTR_BRANCH,
  INSTR_BLOCK_DATA_TRANSFER,
  INSTR_COPROCESSOR_REGISTER_TRANSFER,
  INSTR_SOFTWARE_INTERRUPT, // TODO
  // N/A - we only have CP15, which doesn't support these two
  INSTR_COPROCESSOR_DATA_TRANSFER,
  INSTR_COPROCESSOR_DATA_OPERATION
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
// Halfword data transfer
///////////////////////////////////////////
typedef struct {
  bool add_offset_before_transfer;
  bool add_offset;
  bool write_back_address;
  bool load;
  pt_arm_register base;
  pt_arm_register source_dest;
  bool is_immediate_offset;
  uint8_t offset_imm;
  pt_arm_register offset_reg;
  bool is_signed;
  bool transfer_byte;
} pt_arm_instr_halfword_data_transfer;

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
// Swap
///////////////////////////////////////////
typedef struct {
  bool transfer_byte;
  pt_arm_register value;
  pt_arm_register dest;
  pt_arm_register addr;
} pt_arm_instr_swap;

///////////////////////////////////////////
// Block data transfer
///////////////////////////////////////////
typedef struct {
  bool add_offset_before_transfer;
  bool add_offset;
  bool load_psr_or_force_user_mode; // TODO - we don't seem to use this?
  bool write_back_address;
  bool load;
  pt_arm_register base;
  uint16_t register_list;
} pt_arm_instr_block_data_transfer;

///////////////////////////////////////////
// Coprocessor register transfer
///////////////////////////////////////////
typedef struct {
  bool load;
  pt_arm_register source_dest;
  uint8_t cp_reg;
  uint8_t cp_num;
  uint8_t opcode_2;
  uint8_t crm;
} pt_arm_instr_coprocessor_register_transfer;

///////////////////////////////////////////
// Multiply
///////////////////////////////////////////
typedef struct {
  pt_arm_register rd;
  pt_arm_register rm;
  pt_arm_register rs;
  pt_arm_register rn;
  bool set_condition_codes;
  bool accumulate;
} pt_arm_instr_multiply;

///////////////////////////////////////////
// Multiply long
///////////////////////////////////////////
typedef struct {
  pt_arm_register rd_hi;
  pt_arm_register rd_lo;
  pt_arm_register rs;
  pt_arm_register rm;
  bool set_condition_codes;
  bool accumulate;
  bool is_signed;
} pt_arm_instr_multiply_long;

///////////////////////////////////////////

typedef union {
  pt_arm_instr_data_processing data_processing;
  pt_arm_instr_single_data_transfer single_data_transfer;
  pt_arm_instr_halfword_data_transfer halfword_data_transfer;
  pt_arm_instr_branch_exchange branch_exchange;
  pt_arm_instr_branch branch;
  pt_arm_instr_swap swap;
  pt_arm_instr_block_data_transfer block_data_transfer;
  pt_arm_instr_coprocessor_register_transfer coprocessor_register_transfer;
  pt_arm_instr_multiply multiply;
  pt_arm_instr_multiply_long multiply_long;
} pt_arm_instructions;

typedef struct {
  pt_arm_condition cond;
  pt_arm_instruction_type type;
  pt_arm_instructions instr;
  uint32_t raw;
} pt_arm_instruction;

extern pt_arm_instruction_type pt_arm_identify_instruction(uint32_t i);
extern int pt_arm_decode_instruction(pt_arm_instruction* dest, uint32_t i);
extern int pt_arm_execute_instruction(pt_arm_cpu* cpu, pt_arm_instruction* instr);

/**
   SLJIT compiler context type. Abstracted in order that one could pentially compile with different
   JIT backends in the future.
 */
typedef struct sljit_compiler* jit;

/**
   Emits a JIT instruction.
   @param context JIT compiler context
   @param cpu Instance of CPU to emit instruction for
   @param instr ARM instruction to emit
   @return 0 for success
 */
extern int pt_arm_emit_instruction(jit context, pt_arm_cpu* cpu, pt_arm_instruction* instr);

extern uint32_t _petraea_eval_operand2(pt_arm_cpu* cpu, pt_arm_operand2* operand2, bool* carryValid, bool* carry);
extern bool _petraea_eval_condition(pt_arm_cpu* cpu, pt_arm_condition cond);

#endif

