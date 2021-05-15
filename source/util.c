#include "util.h"

uint32_t rrot32(uint32_t v, unsigned int by) {
  return (v << (32-by)) | (v >> by);
}


