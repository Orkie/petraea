#include "cpu.h"
#include "instr.h"

static void _init_regs(pt_arm_cpu* cpu);
static uint8_t _fetch_byte(pt_arm_cpu* cpu, uint32_t address);
static uint16_t _fetch_halfword(pt_arm_cpu* cpu, uint32_t address);
static uint32_t _fetch_word(pt_arm_cpu* cpu, uint32_t address);

int pt_arm_init_cpu(pt_arm_cpu* cpu,
		 uint32_t (*bus_fetch_word)(uint32_t),
		 uint16_t (*bus_fetch_halfword)(uint32_t),
		 uint8_t (*bus_fetch_byte)(uint32_t)) {
  _init_regs(cpu);
  cpu->bus_fetch_word = bus_fetch_word;
  cpu->bus_fetch_halfword = bus_fetch_halfword;
  cpu->bus_fetch_byte = bus_fetch_byte;
  cpu->fetch_word = _fetch_word;
  cpu->fetch_halfword = _fetch_halfword;
  cpu->fetch_byte = _fetch_byte;
  return 0;
}

pt_arm_mode pt_arm_current_mode(pt_arm_cpu* cpu) {
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

void pt_arm_set_mode(pt_arm_cpu* cpu, pt_arm_mode mode) {
  switch(mode) {
  case MODE_USER: cpu->cpsr = (cpu->cpsr&0xF) | 0b0000; break;
  case MODE_FIQ: cpu->cpsr = (cpu->cpsr&0xF) | 0b0001; break;
  case MODE_IRQ: cpu->cpsr = (cpu->cpsr&0xF) | 0b0010; break;
  case MODE_SVC: cpu->cpsr = (cpu->cpsr&0xF) | 0b0011; break;
  case MODE_ABT: cpu->cpsr = (cpu->cpsr&0xF) | 0b0111; break;
  case MODE_UND: cpu->cpsr = (cpu->cpsr&0xF) | 0b1011; break;
  case MODE_SYSTEM: cpu->cpsr = (cpu->cpsr&0xF) | 0b1111; break;
  }
}

bool arm_is_little_endian(pt_arm_cpu* cpu) {
  return ((cpu->cpsr >> 9)&0x1) == 0x0;
}

// TODO - implement MMU/PU here
static uint8_t _fetch_byte(pt_arm_cpu* cpu, uint32_t address) {
  return 0;
}

static uint16_t _fetch_halfword(pt_arm_cpu* cpu, uint32_t address) {
  return 0;
}

static uint32_t _fetch_word(pt_arm_cpu* cpu, uint32_t address) {
  const uint32_t value = (*cpu->bus_fetch_word)(address);
  // TODO - handle endianess, and MMU, PU
  return value;
}

pt_arm_registers* pt_arm_get_regs(pt_arm_cpu* cpu) {
  pt_arm_mode mode = pt_arm_current_mode(cpu);
  return &cpu->regs[mode];
}

int pt_arm_clock(pt_arm_cpu* cpu) {
  pt_arm_mode mode = pt_arm_current_mode(cpu);
  uint32_t* pcReg = cpu->regs[mode].regs[REG_PC];
  
  // fetch
  uint32_t pc = *pcReg;
  uint32_t instruction = _fetch_word(cpu, pc);
  
  // decode
  // TODO - thumb
  pt_arm_instruction decoded;
  if(pt_arm_decode_instruction(&decoded, instruction) < 0) {
    return -1;
  }
  
  // execute
  *pcReg = pc+8;
  if(pt_arm_execute_instruction(cpu, &decoded) < 0) {
    return -2;
  }

  // no branch has been executed
  if(*pcReg == (pc+8)) {
    *pcReg = pc+4;
  }

  return 0;
}

static void _init_regs(pt_arm_cpu* cpu) {
  cpu->r0 = 0;
  cpu->r1 = 0;
  cpu->r2 = 0;
  cpu->r3 = 0;
  cpu->r4 = 0;
  cpu->r5 = 0;
  cpu->r6 = 0;
  cpu->r7 = 0;
  cpu->r8 = 0;
  cpu->r9 = 0;
  cpu->r10 = 0;
  cpu->r11 = 0;
  cpu->r12 = 0;
  cpu->r13 = 0;
  cpu->r14 = 0;
  cpu->r15 = 0;
  cpu->r8_fiq = 0;
  cpu->r9_fiq = 0;
  cpu->r10_fiq = 0;
  cpu->r11_fiq = 0;
  cpu->r12_fiq = 0;
  cpu->r13_svc = 0;
  cpu->r13_abt = 0;
  cpu->r13_und = 0;
  cpu->r13_irq = 0;
  cpu->r13_fiq = 0;
  cpu->r14_svc = 0;
  cpu->r14_abt = 0;
  cpu->r14_und = 0;
  cpu->r14_irq = 0;
  cpu->r14_fiq = 0;
  cpu->cpsr = 0;
  cpu->spsr_svc = 0;
  cpu->spsr_abt = 0;
  cpu->spsr_und = 0;
  cpu->spsr_irq = 0;
  cpu->spsr_fiq = 0;
  
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

