#include "cpu.h"

static void _init_regs(__arm_cpu* cpu);

int arm_init_cpu(__arm_cpu* cpu) {
  _init_regs(cpu);
  return 0;
}

__arm_mode arm_current_mode(__arm_cpu* cpu) {
  switch(cpu->cpsr&0xF) {
  case 0b0000: return MODE_USER;
  case 0b0001: return MODE_FIQ;
  case 0b0010: return MODE_IRQ;
  case 0b0011: return MODE_SVC;
  case 0b0111: return MODE_ABT;
  case 0b1011: return MODE_UND;
  case 0b1111: return MODE_SYSTEM;
  }
  return 7; // unknown mode - maybe we should log this somehow rather than just causing a segfault?
}

static void _init_regs(__arm_cpu* cpu) {
  cpu->regs[MODE_USER].regs[0] = &cpu->r0;
  cpu->regs[MODE_USER].regs[1] = &cpu->r1;
  cpu->regs[MODE_USER].regs[2] = &cpu->r2;
  cpu->regs[MODE_USER].regs[3] = &cpu->r3;
  cpu->regs[MODE_USER].regs[4] = &cpu->r4;
  cpu->regs[MODE_USER].regs[5] = &cpu->r5;
  cpu->regs[MODE_USER].regs[6] = &cpu->r6;
  cpu->regs[MODE_USER].regs[7] = &cpu->r7;
  cpu->regs[MODE_USER].regs[8] = &cpu->r8;
  cpu->regs[MODE_USER].regs[9] = &cpu->r9;
  cpu->regs[MODE_USER].regs[10] = &cpu->r10;
  cpu->regs[MODE_USER].regs[11] = &cpu->r11;
  cpu->regs[MODE_USER].regs[12] = &cpu->r12;
  cpu->regs[MODE_USER].regs[13] = &cpu->r13;
  cpu->regs[MODE_USER].regs[14] = &cpu->r14;
  cpu->regs[MODE_USER].regs[15] = &cpu->r15;
  cpu->regs[MODE_USER].cpsr = &cpu->cpsr;
  cpu->regs[MODE_USER].spsr = &cpu->cpsr;

  cpu->regs[MODE_SYSTEM].regs[0] = &cpu->r0;
  cpu->regs[MODE_SYSTEM].regs[1] = &cpu->r1;
  cpu->regs[MODE_SYSTEM].regs[2] = &cpu->r2;
  cpu->regs[MODE_SYSTEM].regs[3] = &cpu->r3;
  cpu->regs[MODE_SYSTEM].regs[4] = &cpu->r4;
  cpu->regs[MODE_SYSTEM].regs[5] = &cpu->r5;
  cpu->regs[MODE_SYSTEM].regs[6] = &cpu->r6;
  cpu->regs[MODE_SYSTEM].regs[7] = &cpu->r7;
  cpu->regs[MODE_SYSTEM].regs[8] = &cpu->r8;
  cpu->regs[MODE_SYSTEM].regs[9] = &cpu->r9;
  cpu->regs[MODE_SYSTEM].regs[10] = &cpu->r10;
  cpu->regs[MODE_SYSTEM].regs[11] = &cpu->r11;
  cpu->regs[MODE_SYSTEM].regs[12] = &cpu->r12;
  cpu->regs[MODE_SYSTEM].regs[13] = &cpu->r13;
  cpu->regs[MODE_SYSTEM].regs[14] = &cpu->r14;
  cpu->regs[MODE_SYSTEM].regs[15] = &cpu->r15;
  cpu->regs[MODE_SYSTEM].cpsr = &cpu->cpsr;
  cpu->regs[MODE_SYSTEM].spsr = &cpu->cpsr;

  cpu->regs[MODE_SVC].regs[0] = &cpu->r0;
  cpu->regs[MODE_SVC].regs[1] = &cpu->r1;
  cpu->regs[MODE_SVC].regs[2] = &cpu->r2;
  cpu->regs[MODE_SVC].regs[3] = &cpu->r3;
  cpu->regs[MODE_SVC].regs[4] = &cpu->r4;
  cpu->regs[MODE_SVC].regs[5] = &cpu->r5;
  cpu->regs[MODE_SVC].regs[6] = &cpu->r6;
  cpu->regs[MODE_SVC].regs[7] = &cpu->r7;
  cpu->regs[MODE_SVC].regs[8] = &cpu->r8;
  cpu->regs[MODE_SVC].regs[9] = &cpu->r9;
  cpu->regs[MODE_SVC].regs[10] = &cpu->r10;
  cpu->regs[MODE_SVC].regs[11] = &cpu->r11;
  cpu->regs[MODE_SVC].regs[12] = &cpu->r12;
  cpu->regs[MODE_SVC].regs[13] = &cpu->r13_svc;
  cpu->regs[MODE_SVC].regs[14] = &cpu->r14_svc;
  cpu->regs[MODE_SVC].regs[15] = &cpu->r15;
  cpu->regs[MODE_SVC].cpsr = &cpu->cpsr;
  cpu->regs[MODE_SVC].spsr = &cpu->spsr_svc;

  cpu->regs[MODE_ABT].regs[0] = &cpu->r0;
  cpu->regs[MODE_ABT].regs[1] = &cpu->r1;
  cpu->regs[MODE_ABT].regs[2] = &cpu->r2;
  cpu->regs[MODE_ABT].regs[3] = &cpu->r3;
  cpu->regs[MODE_ABT].regs[4] = &cpu->r4;
  cpu->regs[MODE_ABT].regs[5] = &cpu->r5;
  cpu->regs[MODE_ABT].regs[6] = &cpu->r6;
  cpu->regs[MODE_ABT].regs[7] = &cpu->r7;
  cpu->regs[MODE_ABT].regs[8] = &cpu->r8;
  cpu->regs[MODE_ABT].regs[9] = &cpu->r9;
  cpu->regs[MODE_ABT].regs[10] = &cpu->r10;
  cpu->regs[MODE_ABT].regs[11] = &cpu->r11;
  cpu->regs[MODE_ABT].regs[12] = &cpu->r12;
  cpu->regs[MODE_ABT].regs[13] = &cpu->r13_abt;
  cpu->regs[MODE_ABT].regs[14] = &cpu->r14_abt;
  cpu->regs[MODE_ABT].regs[15] = &cpu->r15;
  cpu->regs[MODE_ABT].cpsr = &cpu->cpsr;
  cpu->regs[MODE_ABT].spsr = &cpu->spsr_abt;

  cpu->regs[MODE_UND].regs[0] = &cpu->r0;
  cpu->regs[MODE_UND].regs[1] = &cpu->r1;
  cpu->regs[MODE_UND].regs[2] = &cpu->r2;
  cpu->regs[MODE_UND].regs[3] = &cpu->r3;
  cpu->regs[MODE_UND].regs[4] = &cpu->r4;
  cpu->regs[MODE_UND].regs[5] = &cpu->r5;
  cpu->regs[MODE_UND].regs[6] = &cpu->r6;
  cpu->regs[MODE_UND].regs[7] = &cpu->r7;
  cpu->regs[MODE_UND].regs[8] = &cpu->r8;
  cpu->regs[MODE_UND].regs[9] = &cpu->r9;
  cpu->regs[MODE_UND].regs[10] = &cpu->r10;
  cpu->regs[MODE_UND].regs[11] = &cpu->r11;
  cpu->regs[MODE_UND].regs[12] = &cpu->r12;
  cpu->regs[MODE_UND].regs[13] = &cpu->r13_und;
  cpu->regs[MODE_UND].regs[14] = &cpu->r14_und;
  cpu->regs[MODE_UND].regs[15] = &cpu->r15;
  cpu->regs[MODE_UND].cpsr = &cpu->cpsr;
  cpu->regs[MODE_UND].spsr = &cpu->spsr_und;

  cpu->regs[MODE_IRQ].regs[0] = &cpu->r0;
  cpu->regs[MODE_IRQ].regs[1] = &cpu->r1;
  cpu->regs[MODE_IRQ].regs[2] = &cpu->r2;
  cpu->regs[MODE_IRQ].regs[3] = &cpu->r3;
  cpu->regs[MODE_IRQ].regs[4] = &cpu->r4;
  cpu->regs[MODE_IRQ].regs[5] = &cpu->r5;
  cpu->regs[MODE_IRQ].regs[6] = &cpu->r6;
  cpu->regs[MODE_IRQ].regs[7] = &cpu->r7;
  cpu->regs[MODE_IRQ].regs[8] = &cpu->r8;
  cpu->regs[MODE_IRQ].regs[9] = &cpu->r9;
  cpu->regs[MODE_IRQ].regs[10] = &cpu->r10;
  cpu->regs[MODE_IRQ].regs[11] = &cpu->r11;
  cpu->regs[MODE_IRQ].regs[12] = &cpu->r12;
  cpu->regs[MODE_IRQ].regs[13] = &cpu->r13_irq;
  cpu->regs[MODE_IRQ].regs[14] = &cpu->r14_irq;
  cpu->regs[MODE_IRQ].regs[15] = &cpu->r15;
  cpu->regs[MODE_IRQ].cpsr = &cpu->cpsr;
  cpu->regs[MODE_IRQ].spsr = &cpu->spsr_irq;

  cpu->regs[MODE_FIQ].regs[0] = &cpu->r0;
  cpu->regs[MODE_FIQ].regs[1] = &cpu->r1;
  cpu->regs[MODE_FIQ].regs[2] = &cpu->r2;
  cpu->regs[MODE_FIQ].regs[3] = &cpu->r3;
  cpu->regs[MODE_FIQ].regs[4] = &cpu->r4;
  cpu->regs[MODE_FIQ].regs[5] = &cpu->r5;
  cpu->regs[MODE_FIQ].regs[6] = &cpu->r6;
  cpu->regs[MODE_FIQ].regs[7] = &cpu->r7;
  cpu->regs[MODE_FIQ].regs[8] = &cpu->r8_fiq;
  cpu->regs[MODE_FIQ].regs[9] = &cpu->r9_fiq;
  cpu->regs[MODE_FIQ].regs[10] = &cpu->r10_fiq;
  cpu->regs[MODE_FIQ].regs[11] = &cpu->r11_fiq;
  cpu->regs[MODE_FIQ].regs[12] = &cpu->r12_fiq;
  cpu->regs[MODE_FIQ].regs[13] = &cpu->r13_fiq;
  cpu->regs[MODE_FIQ].regs[14] = &cpu->r14_fiq;
  cpu->regs[MODE_FIQ].regs[15] = &cpu->r15;
  cpu->regs[MODE_FIQ].cpsr = &cpu->cpsr;
  cpu->regs[MODE_FIQ].spsr = &cpu->spsr_fiq;
}

