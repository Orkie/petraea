#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <stddef.h>
#include "instr.h"

Test(pt_x64_execute, test) {
  pt_arm_cpu cpu;
  pt_arm_instruction instr;
  instr.type = INSTR_HALFWORD_DATA_TRANSFER;
  instr.cond = COND_AL;
  instr.raw = COND_NV<<28;

  pt_arm_execute_instruction(&cpu, &instr);
}