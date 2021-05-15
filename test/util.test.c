#include <criterion/criterion.h>
#include "util.h"

Test(rrot32, can_rotate_1) {
  cr_assert_eq(rrot32(1, 1), 0x80000000);
  cr_assert_eq(rrot32(0x12345678, 4), 0x81234567);
  cr_assert_eq(rrot32(0x12345678, 8), 0x78123456);
  cr_assert_eq(rrot32(0x12345678, 12), 0x67812345);
  cr_assert_eq(rrot32(0x12345678, 16), 0x56781234);
  cr_assert_eq(rrot32(0x12345678, 20), 0x45678123);
  cr_assert_eq(rrot32(0x12345678, 24), 0x34567812);
  cr_assert_eq(rrot32(0x12345678, 28), 0x23456781);
  cr_assert_eq(rrot32(0x12345678, 32), 0x12345678);
}

