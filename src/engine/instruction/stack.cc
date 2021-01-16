#include "src/engine/instruction/stack.h"

#include "src/engine/public/constants.h"

uint8_t PHA::Execute(uint16_t operand) {
  bus_.Write(kStackStartAddress + reg_.sp--, reg_.acc);
  return 0;
}

uint8_t PLA::Execute(uint16_t operand) {
  bus_.Read(kStackStartAddress + ++reg_.sp, &reg_.acc);
  return 0;
}

uint8_t PHP::Execute(uint16_t operand) {
  bus_.Write(kStackStartAddress + reg_.sp--, reg_.status.to_ulong());
  return 0;
}

uint8_t PLP::Execute(uint16_t operand) {
  uint8_t byte_read;
  bus_.Read(kStackStartAddress + ++reg_.sp, &byte_read);
  reg_.status = byte_read;
  return 0;
}
