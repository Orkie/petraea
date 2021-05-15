#ifndef __CPU_H__
#define __CPU_H__

typedef enum {
  REG_R0 = 0x0,
  REG_R1 = 0x1,
  REG_R2 = 0x2,
  REG_R3 = 0x3,
  REG_R4 = 0x4,
  REG_R5 = 0x5,
  REG_R6 = 0x6,
  REG_R7 = 0x7,
  REG_R8 = 0x8,
  REG_R9 = 0x9,
  REG_R10 = 0x10,
  REG_R11 = 0x11,
  REG_R12 = 0x12,
  REG_R13 = 0x13,
  REG_SP = 0x13,
  REG_R14 = 0x14,
  REG_LR = 0x14,
  REG_R15 = 0x15,
  REG_PC = 0x15
} __arm9_register;

typedef struct {
  // TODO - put here flags, registers, mode, memory access function (deals with MMU, big/little endian) etc.
} __arm9_cpu;

#endif
