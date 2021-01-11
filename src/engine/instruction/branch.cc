#include "src/engine/instruction/branch.h"

#include <iostream>
#include <optional>

namespace {

std::optional<uint8_t> Branch(Registers& reg, Status condition, bool set,
                              int cycle, uint16_t operand) {
  if (cycle == 2) {
    reg.pc++;
    if (reg.status.test(condition) != set) {
      return 0;
    } else {
      return std::nullopt;
    }
  }

  else if (cycle == 3) {
    uint8_t uoffset = operand & 0xFF;
    int8_t offset = static_cast<int8_t>(uoffset);

    // Run an extra cycle if we cross the page boundary.
    std::optional<uint8_t> finished = 0;
    uint8_t pcl = reg.pc & 0xFF;
    if ((pcl > 0x100 - uoffset) || (pcl < uoffset)) {
      finished = std::nullopt;
    }

    reg.pc += offset;
    return finished;
  }

  return 0;
}

}  // namespace

std::optional<uint8_t> BPL::Execute(uint8_t opcode, Instruction2::Mode mode,
                                    uint16_t operand, int cycle) {
  return Branch(reg_, Status::kSign, false, cycle, operand);
}

std::optional<uint8_t> BMI::Execute(uint8_t opcode, Instruction2::Mode mode,
                                    uint16_t operand, int cycle) {
  return Branch(reg_, Status::kSign, true, cycle, operand);
}

std::optional<uint8_t> BVC::Execute(uint8_t opcode, Instruction2::Mode mode,
                                    uint16_t operand, int cycle) {
  return Branch(reg_, Status::kOverflow, false, cycle, operand);
}

std::optional<uint8_t> BVS::Execute(uint8_t opcode, Instruction2::Mode mode,
                                    uint16_t operand, int cycle) {
  return Branch(reg_, Status::kOverflow, true, cycle, operand);
}

std::optional<uint8_t> BCC::Execute(uint8_t opcode, Instruction2::Mode mode,
                                    uint16_t operand, int cycle) {
  return Branch(reg_, Status::kCarry, false, cycle, operand);
}

std::optional<uint8_t> BCS::Execute(uint8_t opcode, Instruction2::Mode mode,
                                    uint16_t operand, int cycle) {
  return Branch(reg_, Status::kCarry, true, cycle, operand);
}

std::optional<uint8_t> BNE::Execute(uint8_t opcode, Instruction2::Mode mode,
                                    uint16_t operand, int cycle) {
  return Branch(reg_, Status::kZero, false, cycle, operand);
}

std::optional<uint8_t> BEQ::Execute(uint8_t opcode, Instruction2::Mode mode,
                                    uint16_t operand, int cycle) {
  return Branch(reg_, Status::kZero, true, cycle, operand);
}
