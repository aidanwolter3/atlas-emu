#include "src/instruction/status.h"

void SEI::Execute(uint8_t opcode) {
  auto s = cpu_.GetStatusRegister();
  s.int_disable = true;
  cpu_.SetStatusRegister(s);
}

void CLD::Execute(uint8_t opcode) {
  auto s = cpu_.GetStatusRegister();
  s.bcd_mode = false;
  cpu_.SetStatusRegister(s);
}
