#include "src/engine/instruction/status.h"

bool CLC::Execute(uint8_t opcode, Addressing::Mode mode, uint16_t operand,
                  int cycle) {
  if (cycle < 2) return false;
  reg_.status.reset(Status::kCarry);
  return true;
}

bool SEC::Execute(uint8_t opcode, Addressing::Mode mode, uint16_t operand,
                  int cycle) {
  if (cycle < 2) return false;
  reg_.status.set(Status::kCarry);
  return true;
}

bool CLI::Execute(uint8_t opcode, Addressing::Mode mode, uint16_t operand,
                  int cycle) {
  if (cycle < 2) return false;
  reg_.status.reset(Status::kIntDisable);
  return true;
}

bool SEI::Execute(uint8_t opcode, Addressing::Mode mode, uint16_t operand,
                  int cycle) {
  if (cycle < 2) return false;
  reg_.status.set(Status::kIntDisable);
  return true;
}

bool CLV::Execute(uint8_t opcode, Addressing::Mode mode, uint16_t operand,
                  int cycle) {
  if (cycle < 2) return false;
  reg_.status.reset(Status::kOverflow);
  return true;
}

bool CLD::Execute(uint8_t opcode, Addressing::Mode mode, uint16_t operand,
                  int cycle) {
  if (cycle < 2) return false;
  reg_.status.reset(Status::kBCDMode);
  return true;
}

bool SED::Execute(uint8_t opcode, Addressing::Mode mode, uint16_t operand,
                  int cycle) {
  if (cycle < 2) return false;
  reg_.status.set(Status::kBCDMode);
  return true;
}
