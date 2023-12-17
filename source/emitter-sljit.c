#include <sljitLir.h>
#include "instr.h"

// SLJIT_S0 = CPU structure

static inline __attribute__((always_inline)) int emit_data_processing(jit C, pt_arm_cpu* cpu, pt_arm_instr_data_processing* i) {

  // this reads the dest register and puts the vlaue in R0
  sljit_emit_op1(C, SLJIT_MOV_P, SLJIT_R0, 0, SLJIT_MEM1(SLJIT_S0), SLJIT_OFFSETOF(pt_arm_cpu, currentRegs));
  sljit_emit_op1(C, SLJIT_MOV_P, SLJIT_R0, 0, SLJIT_MEM1(SLJIT_R0), SLJIT_OFFSETOF(pt_arm_registers, regs));
  sljit_emit_op1(C, SLJIT_MOV, SLJIT_R1, 0, SLJIT_MEM1(SLJIT_R0), (sizeof(uint32_t) * i->dest));

  // this writes a new value to dest register
  sljit_emit_op1(C, SLJIT_MOV_P, SLJIT_R0, 0, SLJIT_MEM1(SLJIT_S0), SLJIT_OFFSETOF(pt_arm_cpu, currentRegs));
  sljit_emit_op1(C, SLJIT_MOV_P, SLJIT_R0, 0, SLJIT_MEM1(SLJIT_R0), SLJIT_OFFSETOF(pt_arm_registers, regs));
  sljit_emit_op1(C, SLJIT_MOV_U32, SLJIT_MEM1(SLJIT_R0), (sizeof(uint32_t) * i->dest), SLJIT_IMM, 0x4321);
  
  printf("EMITTING\n");
  
  switch(i->opcode) {
  case OPCODE_AND:
    
  default: return -1;
  }
}
  
int pt_arm_emit_instruction(jit C, pt_arm_cpu* cpu, pt_arm_instruction* instr) {
  //  struct sljit_jump* cond_skip;
  //cond_skip = sljit_emit_jump(C, SLJIT_JUMP);
  
  // TODO - emit condition
  switch(instr->type) {
  case INSTR_DATA_PROCESSING:
    return emit_data_processing(C, cpu, &instr->instr.data_processing);
  default: return -1;
  }

  //sljit_set_label(cond_skip, sljit_emit_label(C));
  
  return 0;
}
