#include "src/instruction/register.h"

namespace {

void SetStatusFromData(Registers& reg, uint8_t data) {
  reg.status.set(Status::kZero, data == 0);
  reg.status.set(Status::kSign, static_cast<int8_t>(data) < 0);
}

}  // namespace

void TAX::ExecuteInternal(uint8_t opcode) {
  reg_.x = reg_.acc;
  SetStatusFromData(reg_, reg_.x);
}

void TXA::ExecuteInternal(uint8_t opcode) {
  reg_.acc = reg_.x;
  SetStatusFromData(reg_, reg_.acc);
}

void DEX::ExecuteInternal(uint8_t opcode) {
  reg_.x -= 1;
  SetStatusFromData(reg_, reg_.x);
}

void INX::ExecuteInternal(uint8_t opcode) {
  reg_.x += 1;
  SetStatusFromData(reg_, reg_.x);
}

void TAY::ExecuteInternal(uint8_t opcode) {
  reg_.y = reg_.acc;
  SetStatusFromData(reg_, reg_.y);
}

void TYA::ExecuteInternal(uint8_t opcode) {
  reg_.acc = reg_.y;
  SetStatusFromData(reg_, reg_.acc);
}

void DEY::ExecuteInternal(uint8_t opcode) {
  reg_.y -= 1;
  SetStatusFromData(reg_, reg_.y);
}

void INY::ExecuteInternal(uint8_t opcode) {
  reg_.y += 1;
  SetStatusFromData(reg_, reg_.y);
}
