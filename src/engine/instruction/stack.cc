#include "src/engine/instruction/stack.h"

#include "src/engine/public/constants.h"

std::optional<uint8_t> PHA::Execute(uint8_t opcode, Instruction2::Mode mode,
                                    uint16_t operand, int cycle) {
  if (cycle < 3) return std::nullopt;
  bus_.Write(kStackStartAddress + reg_.sp--, reg_.acc);
  return 0;
}

std::optional<uint8_t> PLA::Execute(uint8_t opcode, Instruction2::Mode mode,
                                    uint16_t operand, int cycle) {
  if (cycle < 4) return std::nullopt;
  bus_.Read(kStackStartAddress + ++reg_.sp, &reg_.acc);
  return 0;
}

std::optional<uint8_t> PHP::Execute(uint8_t opcode, Instruction2::Mode mode,
                                    uint16_t operand, int cycle) {
  if (cycle < 3) return std::nullopt;
  bus_.Write(kStackStartAddress + reg_.sp--, reg_.status.to_ulong());
  return 0;
}

std::optional<uint8_t> PLP::Execute(uint8_t opcode, Instruction2::Mode mode,
                                    uint16_t operand, int cycle) {
  if (cycle < 4) return std::nullopt;
  uint8_t byte_read;
  bus_.Read(kStackStartAddress + ++reg_.sp, &byte_read);
  reg_.status = byte_read;
  return 0;
}
