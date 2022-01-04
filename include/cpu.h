#ifndef _CPU_H_
#define _CPU_H_

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
} pt_arm_register;

typedef enum {
  ARM920T = 0x920,
  ARM940T = 0x940
} pt_arm_part;

typedef struct {
  uint32_t* regs[16];
  uint32_t* spsr;
  uint32_t* cpsr;
} pt_arm_registers;

typedef enum {
  MODE_USER = 0,
  MODE_SYSTEM = 1,
  MODE_SVC = 2,
  MODE_ABT = 3,
  MODE_UND = 4,
  MODE_IRQ = 5,
  MODE_FIQ = 6
} pt_arm_mode;

struct pt_arm_cpu_struct;
typedef struct pt_arm_cpu_struct pt_arm_cpu;

typedef struct {
  bool present;
  uint32_t (*read)(pt_arm_cpu*, void*, uint8_t, uint8_t, uint8_t);
  void (*write)(pt_arm_cpu*, void*, uint8_t, uint32_t, uint8_t, uint8_t);
  void* state;
} pt_arm_coprocessor;

typedef struct {
  uint32_t id;
  uint32_t cache_type;
} pt_cp15_state;

struct pt_arm_cpu_struct {
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

  pt_arm_registers regs[7];
  pt_arm_coprocessor coprocessors[16];
  pt_cp15_state cp15;

  uint32_t (*bus_fetch_word)(uint32_t);
  uint16_t (*bus_fetch_halfword)(uint32_t);
  uint8_t (*bus_fetch_byte)(uint32_t);

  void (*bus_write_word)(uint32_t, uint32_t);
  void (*bus_write_halfword)(uint32_t, uint16_t);
  void (*bus_write_byte)(uint32_t, uint8_t);

  uint32_t (*fetch_word)(pt_arm_cpu*, uint32_t, bool, bool);
  uint16_t (*fetch_halfword)(pt_arm_cpu*, uint32_t, bool);
  uint8_t (*fetch_byte)(pt_arm_cpu*, uint32_t, bool);
  
  void (*write_word)(pt_arm_cpu*, uint32_t, uint32_t, bool);
  void (*write_halfword)(pt_arm_cpu*, uint32_t, uint16_t, bool);
  void (*write_byte)(pt_arm_cpu*, uint32_t, uint8_t, bool);
  
};


// N
#define GET_NEGATIVE_FLAG(cpu) (((cpu)->cpsr >> 31)&0x1)
#define SET_NEGATIVE_FLAG(cpu, value) (cpu->cpsr = ((cpu->cpsr & (~(1<<31))) | (((value)&1)<<31)))

// Z
#define GET_ZERO_FLAG(cpu) (((cpu)->cpsr >> 30)&0x1)
#define SET_ZERO_FLAG(cpu, value) (cpu->cpsr = ((cpu->cpsr & (~(1<<30))) | (((value)&1)<<30)))

// C
#define GET_CARRY_FLAG(cpu) (((cpu)->cpsr >> 29)&0x1)
#define SET_CARRY_FLAG(cpu, value) (cpu->cpsr = ((cpu->cpsr & (~(1<<29))) | (((value)&1)<<29)))

// V
#define GET_OVERFLOW_FLAG(cpu) (((cpu)->cpsr >> 28)&0x1)
#define SET_OVERFLOW_FLAG(cpu, value) (cpu->cpsr = ((cpu->cpsr & (~(1<<28))) | (((value)&1)<<28)))

// T
#define GET_THUMB_FLAG(cpu) (((cpu)->cpsr >> 5)&0x1)
#define SET_THUMB_FLAG(cpu, value) (cpu->cpsr = ((cpu->cpsr & (~(1<<5))) | (((value)&1)<<5)))

extern int pt_arm_init_cpu(pt_arm_cpu* cpu,
			   pt_arm_part part,
			   uint32_t (*bus_fetch_word)(uint32_t),
			   uint16_t (*bus_fetch_halfword)(uint32_t),
			   uint8_t (*bus_fetch_byte)(uint32_t),
			   void (*bus_write_word)(uint32_t, uint32_t),
			   void (*bus_write_halfword)(uint32_t, uint16_t),
			   void (*bus_write_byte)(uint32_t, uint8_t));
extern pt_arm_mode pt_arm_current_mode(pt_arm_cpu* cpu);
extern void pt_arm_set_mode(pt_arm_cpu* cpu, pt_arm_mode mode);
extern int pt_arm_clock(pt_arm_cpu* cpu);
extern pt_arm_registers* pt_arm_get_regs(pt_arm_cpu* cpu);
extern pt_arm_registers* pt_arm_get_regs_for_mode(pt_arm_cpu* cpu, pt_arm_mode mode);
extern bool pt_arm_is_privileged(pt_arm_cpu* cpu);

#endif
