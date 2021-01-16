#include "src/engine/instruction/load.h"

namespace {

void SetStatusFromData(Registers& reg, uint8_t data) {
  reg.status.set(Status::kZero, data == 0);
  reg.status.set(Status::kSign, static_cast<int8_t>(data) < 0);
}

}  // namespace

uint8_t LDA::Execute(uint8_t opcode, uint16_t operand) {
  reg_.acc = operand & 0xFF;
  SetStatusFromData(reg_, reg_.acc);
  return 0;
}

uint8_t LDX::Execute(uint8_t opcode, uint16_t operand) {
  reg_.x = operand & 0xFF;
  SetStatusFromData(reg_, reg_.x);
  return 0;
}

uint8_t LDY::Execute(uint8_t opcode, uint16_t operand) {
  reg_.y = operand & 0xFF;
  SetStatusFromData(reg_, reg_.y);
  return 0;
}
