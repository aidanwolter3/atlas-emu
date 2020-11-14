#include "src/instruction/status.h"

void CLC::ExecuteInternal(uint8_t opcode) { reg_.status.reset(Status::kCarry); }

void SEC::ExecuteInternal(uint8_t opcode) { reg_.status.set(Status::kCarry); }

void CLI::ExecuteInternal(uint8_t opcode) {
  reg_.status.reset(Status::kIntDisable);
}

void SEI::ExecuteInternal(uint8_t opcode) {
  reg_.status.set(Status::kIntDisable);
}

void CLV::ExecuteInternal(uint8_t opcode) {
  reg_.status.reset(Status::kOverflow);
}

void CLD::ExecuteInternal(uint8_t opcode) {
  reg_.status.reset(Status::kBCDMode);
}

void SED::ExecuteInternal(uint8_t opcode) { reg_.status.set(Status::kBCDMode); }
