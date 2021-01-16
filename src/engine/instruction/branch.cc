#include "src/engine/instruction/branch.h"

namespace {

uint8_t Branch(Registers& reg, Status condition, bool set, uint16_t operand) {
  if (reg.status.test(condition) == set) {
    reg.pc += static_cast<int8_t>(operand & 0xFF);
  }
  return 0;
}

}  // namespace

uint8_t BPL::Execute(uint16_t operand) {
  return Branch(reg_, Status::kSign, false, operand);
}

uint8_t BMI::Execute(uint16_t operand) {
  return Branch(reg_, Status::kSign, true, operand);
}

uint8_t BVC::Execute(uint16_t operand) {
  return Branch(reg_, Status::kOverflow, false, operand);
}

uint8_t BVS::Execute(uint16_t operand) {
  return Branch(reg_, Status::kOverflow, true, operand);
}

uint8_t BCC::Execute(uint16_t operand) {
  return Branch(reg_, Status::kCarry, false, operand);
}

uint8_t BCS::Execute(uint16_t operand) {
  return Branch(reg_, Status::kCarry, true, operand);
}

uint8_t BNE::Execute(uint16_t operand) {
  return Branch(reg_, Status::kZero, false, operand);
}

uint8_t BEQ::Execute(uint16_t operand) {
  return Branch(reg_, Status::kZero, true, operand);
}
