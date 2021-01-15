#include "src/engine/instruction/compare.h"

#include <cstdint>
#include <iostream>

namespace {

uint8_t Compare(Registers& reg, uint8_t value, uint8_t comparison) {
  reg.status.set(Status::kZero, value == comparison);
  reg.status.set(Status::kSign, comparison > value);
  reg.status.set(Status::kCarry, value >= comparison);
  return 0;
}

}  // namespace

uint8_t CMP::Execute(uint8_t opcode, uint16_t operand) {
  return Compare(reg_, reg_.acc, operand);
}

uint8_t CPX::Execute(uint8_t opcode, uint16_t operand) {
  return Compare(reg_, reg_.x, operand);
}

uint8_t CPY::Execute(uint8_t opcode, uint16_t operand) {
  return Compare(reg_, reg_.y, operand);
}
