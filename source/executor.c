#include "cpu.h"
#include "instr.h"

static int execute_branch(__arm_cpu* cpu, __arm_instr_branch* branch) {
  __arm_registers* regs = arm_get_regs(cpu);
  if(branch->link) {
    (*regs->regs)[REG_LR] = (*regs->regs)[REG_PC];
  }
  (*regs->regs)[REG_PC] += branch->offset;
  return 0;
}

int arm_execute_instruction(__arm_cpu* cpu, __arm_instruction* instr) {
  // TODO - cond
  
  switch(instr->type) {
  case INSTR_BRANCH:
    return execute_branch(cpu, &instr->instr.branch);
  default: return -1;
  }
}

