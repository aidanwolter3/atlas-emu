#include "src/engine/instruction/stack.h"

#include "src/engine/base/constants.h"

Instruction::ExecuteResult PHA::Execute(uint16_t operand) {
  bus_.Write(kStackStartAddress + reg_.sp--, reg_.acc);
  return {.cycles = 1};
}

Instruction::ExecuteResult PLA::Execute(uint16_t operand) {
  bus_.Read(kStackStartAddress + ++reg_.sp, &reg_.acc);
  return {.cycles = 2};
}

Instruction::ExecuteResult PHP::Execute(uint16_t operand) {
  bus_.Write(kStackStartAddress + reg_.sp--, reg_.status.to_ulong());
  return {.cycles = 1};
}

Instruction::ExecuteResult PLP::Execute(uint16_t operand) {
  uint8_t byte_read;
  bus_.Read(kStackStartAddress + ++reg_.sp, &byte_read);
  reg_.status = byte_read;
  return {.cycles = 2};
}
