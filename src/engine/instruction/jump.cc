#include "src/engine/instruction/jump.h"

#include <iostream>

#include "src/engine/public/constants.h"

std::optional<uint8_t> JMP::Execute(uint8_t opcode, Instruction2::Mode mode,
                                    uint16_t operand, int cycle) {
  reg_.pc = operand;
  return 0;
}

std::optional<uint8_t> JSR::Execute(uint8_t opcode, Instruction2::Mode mode,
                                    uint16_t operand, int cycle) {
  if (cycle < 6) return std::nullopt;
  uint16_t address_to_push = reg_.pc + 1;
  bus_.Write(kStackStartAddress + reg_.sp--, address_to_push >> 8);
  bus_.Write(kStackStartAddress + reg_.sp--, address_to_push & 0xFF);
  reg_.pc = operand;
  return 0;
}

std::optional<uint8_t> RTS::Execute(uint8_t opcode, Instruction2::Mode mode,
                                    uint16_t operand, int cycle) {
  if (cycle < 6) return std::nullopt;
  uint8_t lower_byte, upper_byte;
  bus_.Read(kStackStartAddress + ++reg_.sp, &lower_byte);
  bus_.Read(kStackStartAddress + ++reg_.sp, &upper_byte);
  uint16_t address_from_stack = (upper_byte << 8) | lower_byte;
  reg_.pc = address_from_stack + 1;
  return 0;
}

std::optional<uint8_t> RTI::Execute(uint8_t opcode, Instruction2::Mode mode,
                                    uint16_t operand, int cycle) {
  if (cycle < 6) return std::nullopt;

  // Pull the status from the stack.
  uint8_t status;
  bus_.Read(kStackStartAddress + ++reg_.sp, &status);
  reg_.status = status;

  // Pull the pc from the stack.
  uint8_t lower_byte, upper_byte;
  bus_.Read(kStackStartAddress + ++reg_.sp, &lower_byte);
  bus_.Read(kStackStartAddress + ++reg_.sp, &upper_byte);
  reg_.pc = (upper_byte << 8) | lower_byte;
  return 0;
}
