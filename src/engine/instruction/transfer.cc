#include "src/engine/instruction/transfer.h"

namespace {

void SetStatusFromData(Registers& reg, uint8_t data) {
  reg.status.set(Status::kZero, data == 0);
  reg.status.set(Status::kSign, static_cast<int8_t>(data) < 0);
}

}  // namespace

bool TAX::Execute(uint8_t opcode, uint16_t operand, int cycle) {
  if (cycle < 2) return false;
  reg_.x = reg_.acc;
  SetStatusFromData(reg_, reg_.x);
  return true;
}

bool TXA::Execute(uint8_t opcode, uint16_t operand, int cycle) {
  if (cycle < 2) return false;
  reg_.acc = reg_.x;
  SetStatusFromData(reg_, reg_.acc);
  return true;
}

bool TAY::Execute(uint8_t opcode, uint16_t operand, int cycle) {
  if (cycle < 2) return false;
  reg_.y = reg_.acc;
  SetStatusFromData(reg_, reg_.y);
  return true;
}

bool TYA::Execute(uint8_t opcode, uint16_t operand, int cycle) {
  if (cycle < 2) return false;
  reg_.acc = reg_.y;
  SetStatusFromData(reg_, reg_.acc);
  return true;
}

bool TXS::Execute(uint8_t opcode, uint16_t operand, int cycle) {
  if (cycle < 2) return false;
  reg_.sp = reg_.x;
  return true;
}

bool TSX::Execute(uint8_t opcode, uint16_t operand, int cycle) {
  if (cycle < 2) return false;
  reg_.x = reg_.sp;
  return true;
}
