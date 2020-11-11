#ifndef PUBLIC_REGISTERS_H_
#define PUBLIC_REGISTERS_H_

#include <bitset>
#include <cstdint>

enum Status {
  kCarry = 0,
  kZero = 1,
  kIntDisable = 2,
  kBCDMode = 3,
  kBrk = 4,
  kOverflow = 5,
  kSign = 6,
};

struct Registers {
  uint16_t pc = 0;
  std::bitset<8> status = 0;
  uint8_t acc = 0;
};

#endif  // PUBLIC_REGISTERS_H_
