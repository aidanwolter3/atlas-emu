#include "src/instruction/status.h"

void SEI::Execute(uint8_t opcode) {
  auto s = GetStatusRegister();
  s.int_disable = true;
  SetStatusRegister(s);
}

void CLD::Execute(uint8_t opcode) {
  auto s = GetStatusRegister();
  s.bcd_mode = false;
  SetStatusRegister(s);
}
