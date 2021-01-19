#include "src/engine/instruction/transfer.h"

namespace {

void SetStatusFromData(Registers& reg, uint8_t data) {
  reg.status.set(Status::kZero, data == 0);
  reg.status.set(Status::kSign, static_cast<int8_t>(data) < 0);
}

}  // namespace

Instruction::ExecuteResult TAX::Execute(uint16_t operand) {
  reg_.x = reg_.acc;
  SetStatusFromData(reg_, reg_.x);
  return {};
}

Instruction::ExecuteResult TXA::Execute(uint16_t operand) {
  reg_.acc = reg_.x;
  SetStatusFromData(reg_, reg_.acc);
  return {};
}

Instruction::ExecuteResult TAY::Execute(uint16_t operand) {
  reg_.y = reg_.acc;
  SetStatusFromData(reg_, reg_.y);
  return {};
}

Instruction::ExecuteResult TYA::Execute(uint16_t operand) {
  reg_.acc = reg_.y;
  SetStatusFromData(reg_, reg_.acc);
  return {};
}

Instruction::ExecuteResult TXS::Execute(uint16_t operand) {
  reg_.sp = reg_.x;
  return {};
}

Instruction::ExecuteResult TSX::Execute(uint16_t operand) {
  reg_.x = reg_.sp;
  return {};
}
