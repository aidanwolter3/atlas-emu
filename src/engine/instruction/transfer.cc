#include "src/engine/instruction/transfer.h"

#include <optional>

namespace {

void SetStatusFromData(Registers& reg, uint8_t data) {
  reg.status.set(Status::kZero, data == 0);
  reg.status.set(Status::kSign, static_cast<int8_t>(data) < 0);
}

}  // namespace

uint8_t TAX::Execute(uint8_t opcode, uint16_t operand) {
  reg_.x = reg_.acc;
  SetStatusFromData(reg_, reg_.x);
  return 0;
}

uint8_t TXA::Execute(uint8_t opcode, uint16_t operand) {
  reg_.acc = reg_.x;
  SetStatusFromData(reg_, reg_.acc);
  return 0;
}

uint8_t TAY::Execute(uint8_t opcode, uint16_t operand) {
  reg_.y = reg_.acc;
  SetStatusFromData(reg_, reg_.y);
  return 0;
}

uint8_t TYA::Execute(uint8_t opcode, uint16_t operand) {
  reg_.acc = reg_.y;
  SetStatusFromData(reg_, reg_.acc);
  return 0;
}

uint8_t TXS::Execute(uint8_t opcode, uint16_t operand) {
  reg_.sp = reg_.x;
  return 0;
}

uint8_t TSX::Execute(uint8_t opcode, uint16_t operand) {
  reg_.x = reg_.sp;
  return 0;
}
