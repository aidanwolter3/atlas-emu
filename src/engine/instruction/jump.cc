#include "src/engine/instruction/jump.h"

#include "src/engine/base/constants.h"

Instruction::ExecuteResult JMP::Execute(uint16_t operand) {
  reg_.pc = operand;
  return {};
}

Instruction::ExecuteResult JSR::Execute(uint16_t operand) {
  uint16_t address_to_push = reg_.pc - 1;
  bus_.Write(kStackStartAddress + reg_.sp--, address_to_push >> 8);
  bus_.Write(kStackStartAddress + reg_.sp--, address_to_push & 0xFF);
  reg_.pc = operand;
  return {.cycles = 3};
}

Instruction::ExecuteResult RTS::Execute(uint16_t operand) {
  uint8_t lower_byte, upper_byte;
  bus_.Read(kStackStartAddress + ++reg_.sp, &lower_byte);
  bus_.Read(kStackStartAddress + ++reg_.sp, &upper_byte);
  uint16_t address_from_stack = (upper_byte << 8) | lower_byte;
  reg_.pc = address_from_stack + 1;
  return {.cycles = 4};
}

Instruction::ExecuteResult RTI::Execute(uint16_t operand) {
  // Pull the status from the stack.
  uint8_t status;
  bus_.Read(kStackStartAddress + ++reg_.sp, &status);
  reg_.status = status;

  // Pull the pc from the stack.
  uint8_t lower_byte, upper_byte;
  bus_.Read(kStackStartAddress + ++reg_.sp, &lower_byte);
  bus_.Read(kStackStartAddress + ++reg_.sp, &upper_byte);
  reg_.pc = (upper_byte << 8) | lower_byte;
  return {.cycles = 4};
}
