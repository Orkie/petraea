#ifndef __CPU_H__
#define __CPU_H__

#include <stdint.h>
#include <stdbool.h>

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
  uint32_t* regs[16];
  uint32_t* spsr;
  uint32_t* cpsr;
} __arm_registers;

typedef enum {
  MODE_USER,
  MODE_SYSTEM,
  MODE_SVC,
  MODE_ABT,
  MODE_UND,
  MODE_IRQ,
  MODE_FIQ
} __arm_mode;

struct __arm_cpu_struct;
typedef struct __arm_cpu_struct __arm_cpu;
struct __arm_cpu_struct {
  uint32_t r0;
  uint32_t r1;
  uint32_t r2;
  uint32_t r3;
  uint32_t r4;
  uint32_t r5;
  uint32_t r6;
  uint32_t r7;
  uint32_t r8;
  uint32_t r9;
  uint32_t r10;
  uint32_t r11;
  uint32_t r12;
  uint32_t r13;
  uint32_t r14;
  uint32_t r15; 

  uint32_t r8_fiq;
  uint32_t r9_fiq;
  uint32_t r10_fiq;
  uint32_t r11_fiq;
  uint32_t r12_fiq;

  uint32_t r13_svc;
  uint32_t r13_abt;
  uint32_t r13_und;
  uint32_t r13_irq;
  uint32_t r13_fiq;

  uint32_t r14_svc;
  uint32_t r14_abt;
  uint32_t r14_und;
  uint32_t r14_irq;
  uint32_t r14_fiq;
  
  uint32_t cpsr;
  uint32_t spsr_svc;
  uint32_t spsr_abt;
  uint32_t spsr_und;
  uint32_t spsr_irq;
  uint32_t spsr_fiq;

  __arm_registers regs[7];

  uint32_t (*bus_fetch_word)(uint32_t);
  uint16_t (*bus_fetch_halfword)(uint32_t);
  uint8_t (*bus_fetch_byte)(uint32_t);

  uint32_t (*fetch_word)(__arm_cpu*, uint32_t);
  uint16_t (*fetch_halfword)(__arm_cpu*, uint32_t);
  uint8_t (*fetch_byte)(__arm_cpu*, uint32_t);
};

extern int arm_init_cpu(__arm_cpu* cpu,
			uint32_t (*bus_fetch_word)(uint32_t),
			uint16_t (*bus_fetch_halfword)(uint32_t),
			uint8_t (*bus_fetch_byte)(uint32_t));
extern __arm_mode arm_current_mode(__arm_cpu* cpu);
extern int arm_clock(__arm_cpu* cpu);
extern __arm_registers* arm_get_regs(__arm_cpu* cpu);

#endif
