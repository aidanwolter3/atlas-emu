#include "src/instruction/jump.h"

#include <iostream>

namespace {

// TODO: Determine if this constant can be shared with Stack.
const uint16_t kStackStartAddress = 0x100;

}  // namespace

void JMP::ExecuteInternal(uint8_t opcode) {
  uint16_t address;
  switch (opcode) {
    case 0x4C:
      address = Absolute();
      break;
    case 0x6C:
      address = IndirectAbsolute();
      break;
    default:
      std::cout << "Unsupported JMP variant: " << opcode << std::endl;
      return;
  }
  reg_.pc = address;
}

void JSR::ExecuteInternal(uint8_t opcode) {
  uint16_t address = Absolute();
  uint16_t address_to_push = reg_.pc - 1;
  bus_.Write(kStackStartAddress + reg_.sp--, address_to_push >> 8);
  bus_.Write(kStackStartAddress + reg_.sp--, address_to_push & 0xFF);
  reg_.pc = address;
}

void RTS::ExecuteInternal(uint8_t opcode) {
  uint8_t lower_byte, upper_byte;
  bus_.Read(kStackStartAddress + ++reg_.sp, &lower_byte);
  bus_.Read(kStackStartAddress + ++reg_.sp, &upper_byte);
  uint16_t address_from_stack = (upper_byte << 8) | lower_byte;
  reg_.pc = address_from_stack + 1;
}

void RTI::ExecuteInternal(uint8_t opcode) {
  // Pull the status from the stack.
  uint8_t status;
  bus_.Read(kStackStartAddress + ++reg_.sp, &status);
  reg_.status = status;

  // Pull the pc from the stack.
  uint8_t lower_byte, upper_byte;
  bus_.Read(kStackStartAddress + ++reg_.sp, &lower_byte);
  bus_.Read(kStackStartAddress + ++reg_.sp, &upper_byte);
  reg_.pc = (upper_byte << 8) | lower_byte;
}
