#include <stdlib.h>
#include <stdio.h>
#include <criterion/criterion.h>
#include "cpu.h"
#include "instr.h"

Test(executor_branch, can_execute_b_with_positive_offset) {
  __arm_cpu cpu;
  arm_init_cpu(&cpu, NULL, NULL, NULL);
  cpu.r14 = 0xDEADBEEF;
  cpu.r15 = 0x24;
  __arm_instruction instr;
  instr.type = INSTR_BRANCH;
  instr.cond = COND_AL;
  instr.instr.branch.link = false;
  instr.instr.branch.offset = 8;
  
  arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r14, 0xDEADBEEF);
  cr_assert_eq(cpu.r15, 0x24+8);
}

Test(executor_branch, can_execute_b_with_negative_offset) {
  __arm_cpu cpu;
  arm_init_cpu(&cpu, NULL, NULL, NULL);
  cpu.r14 = 0xDEADBEEF;
  cpu.r15 = 0x24;
  __arm_instruction instr;
  instr.type = INSTR_BRANCH;
  instr.cond = COND_AL;
  instr.instr.branch.link = false;
  instr.instr.branch.offset = -8;
  
  arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r14, 0xDEADBEEF);
  cr_assert_eq(cpu.r15, 0x24-8);
}

Test(executor_branch, can_execute_bl_with_positive_offset) {
  __arm_cpu cpu;
  arm_init_cpu(&cpu, NULL, NULL, NULL);
  cpu.r14 = 0xDEADBEEF;
  cpu.r15 = 0x24;
  __arm_instruction instr;
  instr.type = INSTR_BRANCH;
  instr.cond = COND_AL;
  instr.instr.branch.link = true;
  instr.instr.branch.offset = 8;
  
  arm_execute_instruction(&cpu, &instr);

  cr_assert_eq(cpu.r14, 0x24);
  cr_assert_eq(cpu.r15, 0x24+8);
}


