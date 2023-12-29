#include "cpu.h"
#include "instr.h"
#include "util.h"
#include <stdio.h>

extern uint32_t _x64_pt_arm_execute(pt_arm_cpu* cpu, pt_arm_instruction_type type, uint32_t instr);

int pt_arm_execute_instruction(pt_arm_cpu* cpu, pt_arm_instruction* instr) {
  cpu->r1 = 0x12345678;
  uint64_t t = _x64_pt_arm_execute(cpu, instr->type, instr->raw);
  printf("0x%lx (%ld)\n, ", t, t);
  return 0;
}
