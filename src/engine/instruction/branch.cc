#include "src/engine/instruction/branch.h"

namespace {

Instruction::ExecuteResult Branch(Registers& reg, Status condition, bool set,
                                  uint16_t operand) {
  if (reg.status.test(condition) == set) {
    reg.pc += static_cast<int8_t>(operand & 0xFF);
    return {.cycles = 1};
  }
  return {};
}

}  // namespace

Instruction::ExecuteResult BPL::Execute(uint16_t operand) {
  return Branch(reg_, Status::kSign, false, operand);
}

Instruction::ExecuteResult BMI::Execute(uint16_t operand) {
  return Branch(reg_, Status::kSign, true, operand);
}

Instruction::ExecuteResult BVC::Execute(uint16_t operand) {
  return Branch(reg_, Status::kOverflow, false, operand);
}

Instruction::ExecuteResult BVS::Execute(uint16_t operand) {
  return Branch(reg_, Status::kOverflow, true, operand);
}

Instruction::ExecuteResult BCC::Execute(uint16_t operand) {
  return Branch(reg_, Status::kCarry, false, operand);
}

Instruction::ExecuteResult BCS::Execute(uint16_t operand) {
  return Branch(reg_, Status::kCarry, true, operand);
}

Instruction::ExecuteResult BNE::Execute(uint16_t operand) {
  return Branch(reg_, Status::kZero, false, operand);
}

Instruction::ExecuteResult BEQ::Execute(uint16_t operand) {
  return Branch(reg_, Status::kZero, true, operand);
}
