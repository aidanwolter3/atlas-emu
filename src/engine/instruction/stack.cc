#include "src/engine/instruction/stack.h"

#include "src/engine/public/constants.h"

bool PHA::Execute(uint8_t opcode, uint16_t operand, int cycle) {
  if (cycle < 3) return false;
  bus_.Write(kStackStartAddress + reg_.sp--, reg_.acc);
  return true;
}

bool PLA::Execute(uint8_t opcode, uint16_t operand, int cycle) {
  if (cycle < 4) return false;
  bus_.Read(kStackStartAddress + ++reg_.sp, &reg_.acc);
  return true;
}

bool PHP::Execute(uint8_t opcode, uint16_t operand, int cycle) {
  if (cycle < 3) return false;
  bus_.Write(kStackStartAddress + reg_.sp--, reg_.status.to_ulong());
  return true;
}

bool PLP::Execute(uint8_t opcode, uint16_t operand, int cycle) {
  if (cycle < 4) return false;
  uint8_t byte_read;
  bus_.Read(kStackStartAddress + ++reg_.sp, &byte_read);
  reg_.status = byte_read;
  return true;
}
