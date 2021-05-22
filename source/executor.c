#include "cpu.h"
#include "instr.h"
#include "util.h"

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

static int execute_branch(__arm_cpu* cpu, __arm_instr_branch* i) {
  __arm_registers* regs = arm_get_regs(cpu);
  if(i->link) {
    (*regs->regs)[REG_LR] = (*regs->regs)[REG_PC];
  }
  (*regs->regs)[REG_PC] += i->offset;
  return 0;
}

static int execute_data_processing(__arm_cpu* cpu, __arm_instr_data_processing* i) {
  __arm_registers* regs = arm_get_regs(cpu);
  bool shifterCarry = false;
  bool shifterCarryValid = false;
  uint32_t operand1 = (*regs->regs)[i->operand1];
  uint32_t operand2 = eval_operand2(cpu, &i->operand2, &shifterCarryValid, &shifterCarry);
  uint32_t* dest = regs->regs[i->dest];
  uint32_t aluOut;
  switch(i->opcode) {
  case OPCODE_AND:
    *dest = operand1 & operand2;
    if(i->set_condition_codes && i->dest == REG_R15) {
      *regs->cpsr = *regs->spsr;
    } else if(i->set_condition_codes) {
      SET_NEGATIVE_FLAG(cpu, sign32(*dest));
      SET_ZERO_FLAG(cpu, (*dest) == 0);
      if(shifterCarryValid) {
	SET_CARRY_FLAG(cpu, shifterCarry);
      }
    }
    break;
  case OPCODE_EOR:
    *dest = operand1 ^ operand2;
    if(i->set_condition_codes && i->dest == REG_R15) {
      *regs->cpsr = *regs->spsr;
    } else if(i->set_condition_codes) {
      SET_NEGATIVE_FLAG(cpu, sign32(*dest));
      SET_ZERO_FLAG(cpu, (*dest) == 0);
      if(shifterCarryValid) {
	SET_CARRY_FLAG(cpu, shifterCarry);
      }
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
    SET_NEGATIVE_FLAG(cpu, sign32(aluOut));
    SET_ZERO_FLAG(cpu, (aluOut) == 0);
    if(shifterCarryValid) {
      SET_CARRY_FLAG(cpu, shifterCarry);
    }
    break;
  case OPCODE_TEQ: ;
    aluOut = operand1 ^ operand2;
    SET_NEGATIVE_FLAG(cpu, sign32(aluOut));
    SET_ZERO_FLAG(cpu, (aluOut) == 0);
    if(shifterCarryValid) {
      SET_CARRY_FLAG(cpu, shifterCarry);
    }
    break;

  default: return -1;
  }

  return 0;
}

// NOTE - we assume PC here is the same as that of the instruction we are trying to execute - TODO is this a correct assumption? Offset can be 8 or 12 bytes apparently. in the decoder we should store a pc offset at the top level, and apply that before execute. next instruction fetch however needs to deapply it and do +4
int arm_execute_instruction(__arm_cpu* cpu, __arm_instruction* instr) {
  // TODO - cond
  
  switch(instr->type) {
  case INSTR_BRANCH:
    return execute_branch(cpu, &instr->instr.branch);
  case INSTR_DATA_PROCESSING:
    return execute_data_processing(cpu, &instr->instr.data_processing);
  default: return -1;
  }
}

uint32_t eval_operand2(__arm_cpu* cpu, __arm_operand2* operand2, bool* carryValid, bool* carry) {
  if(operand2->is_immediate) {
    *carry = operand2->op.imm.carry;
    *carryValid = operand2->op.imm.carryValid;
    // rotation is handled in the decoder
    return operand2->op.imm.value;
  } else {
    __arm_registers* regs = arm_get_regs(cpu);
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

