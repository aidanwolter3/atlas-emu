#include "src/engine/instruction/branch.h"

namespace {

bool Branch(Registers& reg, Status condition, bool set, int cycle,
            uint16_t operand) {
  if (cycle == 2) {
    if (reg.status.test(condition) != set) {
      reg.pc++;
      return true;
    } else {
      return false;
    }
  }

  else if (cycle == 3) {
    uint8_t uoffset = operand & 0xFF;
    int8_t offset = static_cast<int8_t>(uoffset);

    // Run an extra cycle if we cross the page boundary.
    bool finished = true;
    uint8_t pcl = reg.pc & 0xFF;
    if ((pcl > 0x100 - uoffset) || (pcl < uoffset)) {
      finished = false;
    }

    reg.pc++;
    reg.pc += offset;
    return finished;
  }

  return true;
}

}  // namespace

bool BPL::Execute(uint8_t opcode, uint16_t operand, int cycle) {
  return Branch(reg_, Status::kSign, false, cycle, operand);
}

bool BMI::Execute(uint8_t opcode, uint16_t operand, int cycle) {
  return Branch(reg_, Status::kSign, true, cycle, operand);
}

bool BVC::Execute(uint8_t opcode, uint16_t operand, int cycle) {
  return Branch(reg_, Status::kOverflow, false, cycle, operand);
}

bool BVS::Execute(uint8_t opcode, uint16_t operand, int cycle) {
  return Branch(reg_, Status::kOverflow, true, cycle, operand);
}

bool BCC::Execute(uint8_t opcode, uint16_t operand, int cycle) {
  return Branch(reg_, Status::kCarry, false, cycle, operand);
}

bool BCS::Execute(uint8_t opcode, uint16_t operand, int cycle) {
  return Branch(reg_, Status::kCarry, true, cycle, operand);
}

bool BNE::Execute(uint8_t opcode, uint16_t operand, int cycle) {
  return Branch(reg_, Status::kZero, false, cycle, operand);
}

bool BEQ::Execute(uint8_t opcode, uint16_t operand, int cycle) {
  return Branch(reg_, Status::kZero, true, cycle, operand);
}
