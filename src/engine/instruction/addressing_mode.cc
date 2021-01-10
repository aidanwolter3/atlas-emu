#include "src/engine/instruction/addressing_mode.h"

bool Immediate::FetchOperand(int cycle, uint16_t* operand) {
  uint8_t low;
  bus_.Read(reg_.pc, &low);
  *operand = low;
  return true;
}
