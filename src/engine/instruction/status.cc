#include "src/engine/instruction/status.h"

uint8_t CLC::Execute(uint16_t operand) {
  reg_.status.reset(Status::kCarry);
  return 0;
}

uint8_t SEC::Execute(uint16_t operand) {
  reg_.status.set(Status::kCarry);
  return 0;
}

uint8_t CLI::Execute(uint16_t operand) {
  reg_.status.reset(Status::kIntDisable);
  return 0;
}

uint8_t SEI::Execute(uint16_t operand) {
  reg_.status.set(Status::kIntDisable);
  return 0;
}

uint8_t CLV::Execute(uint16_t operand) {
  reg_.status.reset(Status::kOverflow);
  return 0;
}

uint8_t CLD::Execute(uint16_t operand) {
  reg_.status.reset(Status::kBCDMode);
  return 0;
}

uint8_t SED::Execute(uint16_t operand) {
  reg_.status.set(Status::kBCDMode);
  return 0;
}
