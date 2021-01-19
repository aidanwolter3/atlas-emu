#include "src/engine/instruction/status.h"

Instruction::ExecuteResult CLC::Execute(uint16_t operand) {
  reg_.status.reset(Status::kCarry);
  return {};
}

Instruction::ExecuteResult SEC::Execute(uint16_t operand) {
  reg_.status.set(Status::kCarry);
  return {};
}

Instruction::ExecuteResult CLI::Execute(uint16_t operand) {
  reg_.status.reset(Status::kIntDisable);
  return {};
}

Instruction::ExecuteResult SEI::Execute(uint16_t operand) {
  reg_.status.set(Status::kIntDisable);
  return {};
}

Instruction::ExecuteResult CLV::Execute(uint16_t operand) {
  reg_.status.reset(Status::kOverflow);
  return {};
}

Instruction::ExecuteResult CLD::Execute(uint16_t operand) {
  reg_.status.reset(Status::kBCDMode);
  return {};
}

Instruction::ExecuteResult SED::Execute(uint16_t operand) {
  reg_.status.set(Status::kBCDMode);
  return {};
}
