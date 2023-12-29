#include <stdio.h>
#include <stddef.h>
#include "cpu.h"

#define OFFSET(structure, member) printf("#define OFF_"#structure"_"#member" %d\n", (int) offsetof(structure, member))

int main(int argc, char* argv[]) {
  printf("#ifndef __STRUCT_OFFSETS_H__\n");
  OFFSET(pt_arm_registers, regs);
  OFFSET(pt_arm_cpu, r0);
  OFFSET(pt_arm_cpu, r1);

  #ifdef HOST_ARCH_X64
  OFFSET(pt_arm_cpu, scratch_flags);
  #endif

  printf("#endif\n");
  return 0;
}
