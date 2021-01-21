#include "src/engine/instruction/compare.h"

#include <cstdint>

namespace {

Instruction::ExecuteResult Compare(Registers& reg, uint8_t value,
                                   uint8_t comparison) {
  reg.status.set(Status::kZero, value == comparison);
  reg.status.set(Status::kSign, (value - comparison) & 0xF0);
  reg.status.set(Status::kCarry, value >= comparison);
  return {};
}

}  // namespace

Instruction::ExecuteResult CMP::Execute(uint16_t operand) {
  return Compare(reg_, reg_.acc, operand);
}

Instruction::ExecuteResult CPX::Execute(uint16_t operand) {
  return Compare(reg_, reg_.x, operand);
}

Instruction::ExecuteResult CPY::Execute(uint16_t operand) {
  return Compare(reg_, reg_.y, operand);
}
