#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdint.h>

/**
   Rotate a 32-bit value right by a certain number of bits.
 */
extern uint32_t rrot32(uint32_t v, unsigned int by);

/**
   Returns the sign bit of a 32-bit integer.
 */
#define sign32(v) (((v)>>31)&0x1)

#endif

