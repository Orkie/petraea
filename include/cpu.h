#ifndef __CPU_H__
#define __CPU_H__

typedef enum {
  REG_R0 = 0,
  REG_R1 = 1,
  REG_R2 = 2,
  REG_R3 = 3,
  REG_R4 = 4,
  REG_R5 = 5,
  REG_R6 = 6,
  REG_R7 = 7,
  REG_R8 = 8,
  REG_R9 = 9,
  REG_R10 = 10,
  REG_R11 = 11,
  REG_R12 = 12,
  REG_R13 = 13,
  REG_SP = 13,
  REG_R14 = 14,
  REG_LR = 14,
  REG_R15 = 15,
  REG_PC = 15
} __arm_register;

typedef struct {
  // TODO - put here flags, registers, mode, memory access function (deals with MMU, big/little endian) etc.
} __arm_cpu;

#endif
