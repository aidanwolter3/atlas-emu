#include "src/engine/instruction/logic.h"

namespace {

void SetStatusFromData(Registers& reg, uint8_t data) {
  reg.status.set(Status::kZero, data == 0);
  reg.status.set(Status::kSign, static_cast<int8_t>(data) < 0);
}

}  // namespace

uint8_t AND::Execute(uint16_t operand) {
  uint8_t value = operand & 0xFF;
  reg_.acc &= value;
  SetStatusFromData(reg_, reg_.acc);
  return 0;
}

uint8_t ORA::Execute(uint16_t operand) {
  uint8_t value = operand & 0xFF;
  reg_.acc |= value;
  SetStatusFromData(reg_, reg_.acc);
  return 0;
}

uint8_t EOR::Execute(uint16_t operand) {
  uint8_t value = operand & 0xFF;
  reg_.acc ^= value;
  SetStatusFromData(reg_, reg_.acc);
  return 0;
}

uint8_t BIT::Execute(uint16_t operand) {
  uint8_t value = operand & 0xFF;
  reg_.status.set(Status::kZero, (value & reg_.acc) == 0);
  reg_.status.set(Status::kSign, value & 0x80);
  reg_.status.set(Status::kOverflow, value & 0x40);
  return 0;
}
