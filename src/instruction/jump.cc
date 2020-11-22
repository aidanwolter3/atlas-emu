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
  bus_.Write(kStackStartAddress + reg_.sp--, reg_.pc >> 8);
  bus_.Write(kStackStartAddress + reg_.sp--, reg_.pc & 0xFF);
  reg_.pc = address;
}

void RTS::ExecuteInternal(uint8_t opcode) {
  uint8_t lower_byte, upper_byte;
  bus_.Read(kStackStartAddress + ++reg_.sp, &lower_byte);
  bus_.Read(kStackStartAddress + ++reg_.sp, &upper_byte);
  reg_.pc = (upper_byte << 8) | lower_byte;
}
