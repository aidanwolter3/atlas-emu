#include "src/instruction/status.h"

void SEI::Execute(uint8_t opcode) {
  reg_.status.set(Status::kIntDisable, true);
}

void CLD::Execute(uint8_t opcode) { reg_.status.set(Status::kBCDMode, false); }
