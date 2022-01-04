#include <stdlib.h>
#include <stdio.h>
#include <criterion/criterion.h>
#include "cpu.h"
#include "instr.h"

///////////////////////////////////////////
// ID register
///////////////////////////////////////////

Test(id_reg, arm920t) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL, NULL, NULL, NULL, NULL); 

  pt_arm_instruction instr;
  instr.type = INSTR_COPROCESSOR_REGISTER_TRANSFER;
  instr.cond = COND_AL;
  instr.instr.coprocessor_register_transfer.load = true;
  instr.instr.coprocessor_register_transfer.source_dest = REG_R0;
  instr.instr.coprocessor_register_transfer.cp_reg = 0;
  instr.instr.coprocessor_register_transfer.cp_num = 15;
  instr.instr.coprocessor_register_transfer.opcode_2 = 0;
  instr.instr.coprocessor_register_transfer.crm = 0;

  pt_arm_execute_instruction(&cpu, &instr);
  cr_assert_eq(cpu.r0, 0x41129200);
}

Test(id_reg, arm940t) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM940T, NULL, NULL, NULL, NULL, NULL, NULL); 

  pt_arm_instruction instr;
  instr.type = INSTR_COPROCESSOR_REGISTER_TRANSFER;
  instr.cond = COND_AL;
  instr.instr.coprocessor_register_transfer.load = true;
  instr.instr.coprocessor_register_transfer.source_dest = REG_R0;
  instr.instr.coprocessor_register_transfer.cp_reg = 0;
  instr.instr.coprocessor_register_transfer.cp_num = 15;
  instr.instr.coprocessor_register_transfer.opcode_2 = 0;
  instr.instr.coprocessor_register_transfer.crm = 0;

  pt_arm_execute_instruction(&cpu, &instr);
  cr_assert_eq(cpu.r0, 0x41129400);
}

///////////////////////////////////////////
// Cache type register
///////////////////////////////////////////

Test(cache_type_reg, arm920t) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM920T, NULL, NULL, NULL, NULL, NULL, NULL); 

  pt_arm_instruction instr;
  instr.type = INSTR_COPROCESSOR_REGISTER_TRANSFER;
  instr.cond = COND_AL;
  instr.instr.coprocessor_register_transfer.load = true;
  instr.instr.coprocessor_register_transfer.source_dest = REG_R0;
  instr.instr.coprocessor_register_transfer.cp_reg = 0;
  instr.instr.coprocessor_register_transfer.cp_num = 15;
  instr.instr.coprocessor_register_transfer.opcode_2 = 1;
  instr.instr.coprocessor_register_transfer.crm = 0;

  pt_arm_execute_instruction(&cpu, &instr);
  cr_assert_eq(cpu.r0, 0xD172172);
}

Test(cache_type_reg, arm940t) {
  pt_arm_cpu cpu;
  pt_arm_init_cpu(&cpu, ARM940T, NULL, NULL, NULL, NULL, NULL, NULL); 

  pt_arm_instruction instr;
  instr.type = INSTR_COPROCESSOR_REGISTER_TRANSFER;
  instr.cond = COND_AL;
  instr.instr.coprocessor_register_transfer.load = true;
  instr.instr.coprocessor_register_transfer.source_dest = REG_R0;
  instr.instr.coprocessor_register_transfer.cp_reg = 0;
  instr.instr.coprocessor_register_transfer.cp_num = 15;
  instr.instr.coprocessor_register_transfer.opcode_2 = 1;
  instr.instr.coprocessor_register_transfer.crm = 0;

  pt_arm_execute_instruction(&cpu, &instr);
  cr_assert_eq(cpu.r0, 0xF0F10F1);
}


