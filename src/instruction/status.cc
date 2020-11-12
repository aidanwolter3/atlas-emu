#include "src/instruction/status.h"

void CLC::Execute(uint8_t opcode) { reg_.status.reset(Status::kCarry); }

void SEC::Execute(uint8_t opcode) { reg_.status.set(Status::kCarry); }

void CLI::Execute(uint8_t opcode) { reg_.status.reset(Status::kIntDisable); }

void SEI::Execute(uint8_t opcode) { reg_.status.set(Status::kIntDisable); }

void CLV::Execute(uint8_t opcode) { reg_.status.reset(Status::kOverflow); }

void CLD::Execute(uint8_t opcode) { reg_.status.reset(Status::kBCDMode); }

void SED::Execute(uint8_t opcode) { reg_.status.set(Status::kBCDMode); }
