#include "cpu.h"
#include "instr.h"

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
  uint32_t operand1 = (*regs->regs)[i->operand1];
  uint32_t operand2 = eval_operand2(cpu, &i->operand2);
  
  switch(i->opcode) {
  case OPCODE_AND:
    
    break;
  default: return -1;
  }

  return 0;
}

// NOTE - we assume PC here is the same as that of the instruction we are trying to execute
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

uint32_t eval_operand2(__arm_cpu* cpu, __arm_operand2* operand2) {
  return 0;
}

