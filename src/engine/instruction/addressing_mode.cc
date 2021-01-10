#include "src/engine/instruction/addressing_mode.h"

bool Immediate::FetchOperand(int cycle, uint16_t* operand) {
  if (cycle < 2) return false;
  uint8_t low;
  bus_.Read(reg_.pc, &low);
  *operand = low;
  return true;
}

bool Absolute::FetchOperand(int cycle, uint16_t* operand) {
  if (cycle < 2) return false;
  uint8_t lower, upper;
  bus_.Read(reg_.pc, &lower);
  bus_.Read(reg_.pc + 1, &upper);
  *operand = (upper << 8) | lower;
  return true;
}

bool Indirect::FetchOperand(int cycle, uint16_t* operand) {
  if (cycle < 4) return false;

  uint8_t lower, upper;
  bus_.Read(reg_.pc, &lower);
  bus_.Read(reg_.pc + 1, &upper);

  uint16_t address_location_1 = ((upper << 8) | lower);
  uint16_t address_location_2 =
      ((upper << 8) | static_cast<uint8_t>(lower + 1));

  bus_.Read(address_location_1, &lower);
  bus_.Read(address_location_2, &upper);
  *operand = (upper << 8) | lower;
  return true;
}
