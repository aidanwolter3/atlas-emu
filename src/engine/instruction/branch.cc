#include "src/engine/instruction/branch.h"

#include <iostream>

namespace {

bool Branch(Registers& reg, Bus& bus, Status condition, bool set, int cycle) {
  if (cycle < 2) return false;
  if (cycle == 2) {
    if (reg.status.test(condition) != set) {
      reg.pc++;
      return true;
    } else {
      return false;
    }
  }

  else if (cycle == 3) {
    uint8_t uoffset;
    bus.Read(reg.pc, &uoffset);
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

bool BPL::Execute(uint8_t opcode, int cycle) {
  return Branch(reg_, bus_, Status::kSign, false, cycle);
}

bool BMI::Execute(uint8_t opcode, int cycle) {
  return Branch(reg_, bus_, Status::kSign, true, cycle);
}

bool BVC::Execute(uint8_t opcode, int cycle) {
  return Branch(reg_, bus_, Status::kOverflow, false, cycle);
}

bool BVS::Execute(uint8_t opcode, int cycle) {
  return Branch(reg_, bus_, Status::kOverflow, true, cycle);
}

bool BCC::Execute(uint8_t opcode, int cycle) {
  return Branch(reg_, bus_, Status::kCarry, false, cycle);
}

bool BCS::Execute(uint8_t opcode, int cycle) {
  return Branch(reg_, bus_, Status::kCarry, true, cycle);
}

bool BNE::Execute(uint8_t opcode, int cycle) {
  return Branch(reg_, bus_, Status::kZero, false, cycle);
}

bool BEQ::Execute(uint8_t opcode, int cycle) {
  return Branch(reg_, bus_, Status::kZero, true, cycle);
}
