#include "cpu.h"
#include "instr.h"
#include "util.h"

///////////////////////////////////////////
// Branching
///////////////////////////////////////////

static int execute_branch(pt_arm_cpu* cpu, pt_arm_instr_branch* i) {
  pt_arm_registers* regs = pt_arm_get_regs(cpu);
  if(i->link) {
    (*regs->regs)[REG_LR] = (*regs->regs)[REG_PC]-4;
  }
  (*regs->regs)[REG_PC] += i->offset;
  return 0;
}

static int execute_branch_exchange(pt_arm_cpu* cpu, pt_arm_instr_branch_exchange* i) {
  pt_arm_registers* regs = pt_arm_get_regs(cpu);
  uint32_t address = (*regs->regs)[i->operand];

  SET_THUMB_FLAG(cpu, address&0x1);
  (*regs->regs)[REG_PC] = (address&0xFFFFFFFE);
  return 0;
}

///////////////////////////////////////////
// Data transfer
///////////////////////////////////////////

static int execute_single_data_transfer(pt_arm_cpu* cpu, pt_arm_instr_single_data_transfer* i) {
  pt_arm_registers* regs = pt_arm_get_regs(cpu);
  bool shifterCarry, shifterCarryValid;
  uint32_t offset = _petraea_eval_operand2(cpu, &i->offset, &shifterCarryValid, &shifterCarry);

  uint32_t* baseReg = regs->regs[i->base];
  uint32_t base = *baseReg;
  uint32_t* sourceDestReg = regs->regs[i->source_dest];

  uint32_t modifiedBase = i->add_offset ? (base + offset) : (base - offset);
  uint32_t address = i->add_offset_before_transfer ? modifiedBase : base;

  bool isPrivileged = i->unprivileged ? false : pt_arm_is_privileged(cpu);

  if(i->load) {
    *sourceDestReg = i->transfer_byte
      ? (*cpu->fetch_byte)(cpu, address, isPrivileged)
      : (*cpu->fetch_word)(cpu, address, isPrivileged, true);
  } else {
    if(i->transfer_byte) {
      (*cpu->write_byte)(cpu, address, (*sourceDestReg)&0xFF, isPrivileged);
    } else {
      (*cpu->write_word)(cpu, address, *sourceDestReg, isPrivileged);
    }
  }

  if(i->write_back_address) {
    *baseReg = modifiedBase;
  }

  return 0;
}

static int execute_halfword_data_transfer(pt_arm_cpu* cpu, pt_arm_instr_halfword_data_transfer* i) {
  pt_arm_registers* regs = pt_arm_get_regs(cpu);
  uint32_t offset = i->is_immediate_offset ? i->offset_imm : (*regs->regs)[i->offset_reg];

  uint32_t* baseReg = regs->regs[i->base];
  uint32_t base = *baseReg;
  uint32_t* sourceDestReg = regs->regs[i->source_dest];

  uint32_t modifiedBase = i->add_offset ? (base + offset) : (base - offset);
  uint32_t address = i->add_offset_before_transfer ? modifiedBase : base;

  bool isPrivileged = pt_arm_is_privileged(cpu);

  if(i->load) {
    *sourceDestReg = (i->transfer_byte
       ? (*cpu->fetch_byte)(cpu, address, isPrivileged)
       : (*cpu->fetch_halfword)(cpu, address, isPrivileged));
    if(i->is_signed && i->transfer_byte && (*sourceDestReg&0x80)) {
      *sourceDestReg |= 0xFFFFFF00;
    } else if(i->is_signed && !i->transfer_byte && (*sourceDestReg&0x8000)) {
      *sourceDestReg |= 0xFFFF0000;
    }
  } else {
    if(i->transfer_byte) {
      (*cpu->write_byte)(cpu, address, (*sourceDestReg)&0xFF, isPrivileged);
    } else {
      (*cpu->write_halfword)(cpu, address, *sourceDestReg, isPrivileged);
    }
  }

  if(i->write_back_address) {
    *baseReg = modifiedBase;
  }

  return 0;
}

///////////////////////////////////////////
// ALU
///////////////////////////////////////////

static inline bool subOverflow(int32_t a, int32_t b, bool carry) {
  return (((int64_t)a) - ((int64_t)b) - (carry? 0 : 1)) < ((int32_t)0x80000000);
}

static inline bool addOverflow(int32_t a, int32_t b, bool carry) {
  return (((int64_t)a) + ((int64_t)b) + carry) > 0x7FFFFFFF;
}

static inline bool subBorrow(uint32_t a, uint32_t b, bool carry) {
  return a > (((int64_t)b)+(carry? 1 : 0));
}

static inline bool addCarry(uint32_t a, uint32_t b, bool carry) {
  return (((uint64_t)a) + b + carry) > 0xFFFFFFFF;
}

static inline void handle_flags_logical(pt_arm_cpu* cpu, uint32_t aluOut, bool shifterCarryValid, bool shifterCarry) {
  SET_NEGATIVE_FLAG(cpu, sign32(aluOut));
  SET_ZERO_FLAG(cpu, (aluOut) == 0);
  if(shifterCarryValid) {
    SET_CARRY_FLAG(cpu, shifterCarry);
  }
}

static int execute_data_processing(pt_arm_cpu* cpu, pt_arm_instr_data_processing* i) {
  pt_arm_registers* regs = pt_arm_get_regs(cpu);
  bool shifterCarry = false;
  bool shifterCarryValid = false;
  uint32_t operand1 = (*regs->regs)[i->operand1];
  uint32_t operand2 = _petraea_eval_operand2(cpu, &i->operand2, &shifterCarryValid, &shifterCarry);
  uint32_t* dest = regs->regs[i->dest];
  uint32_t aluOut;
  switch(i->opcode) {
  case OPCODE_AND:
    *dest = operand1 & operand2;
    if(i->set_condition_codes && i->dest == REG_R15) {
      *regs->cpsr = *regs->spsr;
    } else if(i->set_condition_codes) {
      handle_flags_logical(cpu, *dest, shifterCarryValid, shifterCarry);
    }
    break;
  case OPCODE_EOR:
    *dest = operand1 ^ operand2;
    if(i->set_condition_codes && i->dest == REG_R15) {
      *regs->cpsr = *regs->spsr;
    } else if(i->set_condition_codes) {
      handle_flags_logical(cpu, *dest, shifterCarryValid, shifterCarry);
    }
    break;
  case OPCODE_SUB:
    *dest = operand1 - operand2;
    if(i->set_condition_codes && i->dest == REG_R15) {
      *regs->cpsr = *regs->spsr;
    } else if(i->set_condition_codes) {
      SET_NEGATIVE_FLAG(cpu, sign32(*dest));
      SET_ZERO_FLAG(cpu, (*dest) == 0);
      SET_CARRY_FLAG(cpu, subBorrow(operand1, operand2, false));
      SET_OVERFLOW_FLAG(cpu, subOverflow(operand1, operand2, true));
    }
    break;
  case OPCODE_RSB:
    *dest = operand2 - operand1;
    if(i->set_condition_codes && i->dest == REG_R15) {
      *regs->cpsr = *regs->spsr;
    } else if(i->set_condition_codes) {
      SET_NEGATIVE_FLAG(cpu, sign32(*dest));
      SET_ZERO_FLAG(cpu, (*dest) == 0);
      SET_CARRY_FLAG(cpu, subBorrow(operand2, operand1, false));
      SET_OVERFLOW_FLAG(cpu, subOverflow(operand2, operand1, true));
    }
    break;
  case OPCODE_ADD:
    *dest = operand1 + operand2;
    if(i->set_condition_codes && i->dest == REG_R15) {
      *regs->cpsr = *regs->spsr;
    } else if(i->set_condition_codes) {
      SET_NEGATIVE_FLAG(cpu, sign32(*dest));
      SET_ZERO_FLAG(cpu, (*dest) == 0);
      SET_CARRY_FLAG(cpu, addCarry(operand1, operand2, false));
      SET_OVERFLOW_FLAG(cpu, addOverflow(operand1, operand2, false));      
    }
    break;
  case OPCODE_ADC:
    *dest = operand1 + operand2 + GET_CARRY_FLAG(cpu);
    if(i->set_condition_codes && i->dest == REG_R15) {
      *regs->cpsr = *regs->spsr;
    } else if(i->set_condition_codes) {
      bool carry = GET_CARRY_FLAG(cpu);
      SET_NEGATIVE_FLAG(cpu, sign32(*dest));
      SET_ZERO_FLAG(cpu, (*dest) == 0);
      SET_CARRY_FLAG(cpu, addCarry(operand1, operand2, carry));
      SET_OVERFLOW_FLAG(cpu, addOverflow(operand1, operand2, carry));
    }
    break;
  case OPCODE_SBC:
    *dest = operand1 - operand2 - (GET_CARRY_FLAG(cpu) ? 0 : 1);
    if(i->set_condition_codes && i->dest == REG_R15) {
      *regs->cpsr = *regs->spsr;
    } else if(i->set_condition_codes) {
      bool carry = !GET_CARRY_FLAG(cpu);
      SET_NEGATIVE_FLAG(cpu, sign32(*dest));
      SET_ZERO_FLAG(cpu, (*dest) == 0);
      SET_CARRY_FLAG(cpu, subBorrow(operand1, operand2, carry));
      SET_OVERFLOW_FLAG(cpu, subOverflow(operand1, operand2, !carry));
    }
    break;
  case OPCODE_RSC:
    *dest = operand2 - operand1 - (GET_CARRY_FLAG(cpu) ? 0 : 1);
    if(i->set_condition_codes && i->dest == REG_R15) {
      *regs->cpsr = *regs->spsr;
    } else if(i->set_condition_codes) {
      bool carry = !GET_CARRY_FLAG(cpu);
      SET_NEGATIVE_FLAG(cpu, sign32(*dest));
      SET_ZERO_FLAG(cpu, (*dest) == 0);
      SET_CARRY_FLAG(cpu, subBorrow(operand2, operand1, carry));
      SET_OVERFLOW_FLAG(cpu, subOverflow(operand2, operand1, !carry));
    }
    break;
  case OPCODE_TST: ;
    aluOut = operand1 & operand2;
    handle_flags_logical(cpu, aluOut, shifterCarryValid, shifterCarry);
    break;
  case OPCODE_TEQ: ;
    aluOut = operand1 ^ operand2;
    handle_flags_logical(cpu, aluOut, shifterCarryValid, shifterCarry);
    break;
  case OPCODE_CMP: ;
    aluOut = operand1 - operand2;
    SET_NEGATIVE_FLAG(cpu, sign32(aluOut));
    SET_ZERO_FLAG(cpu, (aluOut) == 0);
    SET_CARRY_FLAG(cpu, subBorrow(operand1, operand2, false));
    SET_OVERFLOW_FLAG(cpu, subOverflow(operand1, operand2, true));
    break;
  case OPCODE_CMN: ;
    aluOut = operand1 + operand2;
    SET_NEGATIVE_FLAG(cpu, sign32(aluOut));
    SET_ZERO_FLAG(cpu, (aluOut) == 0);
    SET_CARRY_FLAG(cpu, addCarry(operand1, operand2, false));
    SET_OVERFLOW_FLAG(cpu, addOverflow(operand1, operand2, false));      
    break;
  case OPCODE_ORR:
    *dest = operand1 | operand2;
    if(i->set_condition_codes && i->dest == REG_R15) {
      *regs->cpsr = *regs->spsr;
    } else if(i->set_condition_codes) {
      handle_flags_logical(cpu, *dest, shifterCarryValid, shifterCarry);
    }
    break;
  case OPCODE_MOV:
    *dest = operand2;
    if(i->set_condition_codes && i->dest == REG_R15) {
      *regs->cpsr = *regs->spsr;
    } else if(i->set_condition_codes) {
      handle_flags_logical(cpu, *dest, shifterCarryValid, shifterCarry);
    }
    break;
  case OPCODE_MVN:
    *dest = ~operand2;
    if(i->set_condition_codes && i->dest == REG_R15) {
      *regs->cpsr = *regs->spsr;
    } else if(i->set_condition_codes) {
      handle_flags_logical(cpu, *dest, shifterCarryValid, shifterCarry);
    }
    break;
  case OPCODE_BIC:
    *dest = operand1 & (~operand2);
    if(i->set_condition_codes && i->dest == REG_R15) {
      *regs->cpsr = *regs->spsr;
    } else if(i->set_condition_codes) {
      handle_flags_logical(cpu, *dest, shifterCarryValid, shifterCarry);
    }
    break;
  default: return -1;
  }

  return 0;
}

///////////////////////////////////////////
// Execution utilities
///////////////////////////////////////////
int pt_arm_execute_instruction(pt_arm_cpu* cpu, pt_arm_instruction* instr) {
  if(_petraea_eval_condition(cpu, instr->cond)) {
    switch(instr->type) {
    case INSTR_BRANCH:
      return execute_branch(cpu, &instr->instr.branch);
    case INSTR_BRANCH_EXCHANGE:
      return execute_branch_exchange(cpu, &instr->instr.branch_exchange);
    case INSTR_DATA_PROCESSING:
      return execute_data_processing(cpu, &instr->instr.data_processing);
    case INSTR_SINGLE_DATA_TRANSFER:
      return execute_single_data_transfer(cpu, &instr->instr.single_data_transfer);
    case INSTR_HALFWORD_DATA_TRANSFER:
      return execute_halfword_data_transfer(cpu, &instr->instr.halfword_data_transfer);
    default: return -1;
    }
  }
  return 0;
}

bool _petraea_eval_condition(pt_arm_cpu* cpu, pt_arm_condition cond) {
  switch(cond) {
  case COND_EQ: return GET_ZERO_FLAG(cpu);
  case COND_NE: return !GET_ZERO_FLAG(cpu);
  case COND_CS: return GET_CARRY_FLAG(cpu);
  case COND_CC: return !GET_CARRY_FLAG(cpu);
  case COND_MI: return GET_NEGATIVE_FLAG(cpu);
  case COND_PL: return !GET_NEGATIVE_FLAG(cpu);
  case COND_VS: return GET_OVERFLOW_FLAG(cpu);
  case COND_VC: return !GET_OVERFLOW_FLAG(cpu);
  case COND_HI: return GET_CARRY_FLAG(cpu) && !GET_ZERO_FLAG(cpu);
  case COND_LS: return !GET_CARRY_FLAG(cpu) || GET_ZERO_FLAG(cpu);
  case COND_GE: return GET_NEGATIVE_FLAG(cpu) == GET_OVERFLOW_FLAG(cpu);
  case COND_LT: return GET_NEGATIVE_FLAG(cpu) != GET_OVERFLOW_FLAG(cpu);
  case COND_GT: return !GET_ZERO_FLAG(cpu) && (GET_NEGATIVE_FLAG(cpu) == GET_OVERFLOW_FLAG(cpu));
  case COND_LE: return GET_ZERO_FLAG(cpu) && (GET_NEGATIVE_FLAG(cpu) != GET_OVERFLOW_FLAG(cpu));
  case COND_AL: return true;
  case COND_NV: return false;
  }
  return false;
}

uint32_t _petraea_eval_operand2(pt_arm_cpu* cpu, pt_arm_operand2* operand2, bool* carryValid, bool* carry) {
  if(operand2->is_immediate) {
    *carry = operand2->op.imm.carry;
    *carryValid = operand2->op.imm.carryValid;
    // rotation is handled in the decoder
    return operand2->op.imm.value;
  } else {
    pt_arm_registers* regs = pt_arm_get_regs(cpu);
    uint32_t regValue = (*regs->regs)[operand2->op.reg.reg];
    uint32_t shiftBy = operand2->op.reg.is_register_shift
      ? ((*regs->regs)[operand2->op.reg.shift_reg]&0xFF)
      : operand2->op.reg.shift_imm;

    uint32_t shiftedValue = regValue;
    
    switch(operand2->op.reg.shift_type) {
    case SHIFT_LOGICAL_LEFT:
      if(shiftBy == 0 && !operand2->op.reg.is_register_shift) {
	*carryValid = false;
	*carry = false;
      } else {
	*carry = (shiftedValue >> (32-shiftBy))&0x1;
	*carryValid = true;
	shiftedValue = shiftedValue << shiftBy;
      }
      break;
    case SHIFT_LOGICAL_RIGHT:
      if(shiftBy == 0  && !operand2->op.reg.is_register_shift) {
	*carryValid = true;
	*carry = (shiftedValue>>31)&0x1;
	shiftedValue = 0;
      } else {
	*carryValid = true;
	*carry = (shiftedValue >> (shiftBy-1))&0x1;
	shiftedValue = shiftedValue >> shiftBy;
      }
      break;
    case SHIFT_ARITHMETIC_RIGHT:
      if(shiftBy == 0  && !operand2->op.reg.is_register_shift) {
	*carryValid = true;
	*carry = (shiftedValue>>31)&0x1;
	shiftedValue = (*carry) ? 0xFFFFFFFF : 0x0;
      } else {
	*carryValid = true;
	*carry = (shiftedValue >> (shiftBy-1))&0x1;
	bool top = (shiftedValue>>31)&0x1;
	uint32_t extraBits = top ? (0xFFFFFFFF << (32 - shiftBy)) : 0x0;
	shiftedValue = (shiftedValue >> shiftBy) | extraBits;
      }      
      break;
    case SHIFT_ROTATE_RIGHT:
      if(shiftBy == 0  && !operand2->op.reg.is_register_shift) {
	*carryValid = true;
	*carry = shiftedValue&0x1;
	shiftedValue = (shiftedValue >> 1) | (GET_CARRY_FLAG(cpu) << 31);
      } else {
	*carryValid = true;
	*carry = (shiftedValue >> (shiftBy-1))&0x1;
	shiftedValue = rrot32(shiftedValue, shiftBy);
      }
      break;
    }
    
    return shiftedValue;
  }
}

