#include "src/engine/instruction/addressing.h"

#include <iostream>

Addressing::Addressing(Bus& bus, Registers& reg) : bus_(bus), reg_(reg) {}

bool Addressing::Execute(Mode mode, Operation op, int cycle,
                         uint16_t* operand) {
  switch (mode) {
    case Mode::kImplied:
      return true;
    case Mode::kImmediate:
      return Immediate(op, cycle, operand);
    case Mode::kImmediateAddress:
      return ImmediateAddress(cycle, operand);
    case Mode::kZeroPage:
      return ZeroPage(op, cycle, operand);
    case Mode::kAbsolute:
      return Absolute(op, cycle, operand);
    case Mode::kIndirect:
      return Indirect(op, cycle, operand);
    default:
      std::cout << "Invalid addressing mode: " << (int)mode << std::endl;
      return true;
  }
}

bool Addressing::Immediate(Operation op, int cycle, uint16_t* operand) {
  if (cycle < 2) return false;
  uint8_t lower;
  bus_.Read(reg_.pc, &lower);
  *operand = lower;
  return true;
}

bool Addressing::ImmediateAddress(int cycle, uint16_t* operand) {
  if (cycle < 2) return false;
  uint8_t lower, upper;
  bus_.Read(reg_.pc, &lower);
  bus_.Read(reg_.pc + 1, &upper);
  *operand = (upper << 8) | lower;
  return true;
}

bool Addressing::ZeroPage(Operation op, int cycle, uint16_t* operand) {
  if (cycle < 2) return false;
  uint8_t address_lower, lower;
  bus_.Read(reg_.pc, &address_lower);
  bus_.Read(address_lower, &lower);
  *operand = lower;
  return true;
}

bool Addressing::Absolute(Operation op, int cycle, uint16_t* operand) {
  if (cycle < 2) return false;
  uint8_t address_lower, address_upper, lower, upper;
  bus_.Read(reg_.pc, &address_lower);
  bus_.Read(reg_.pc + 1, &address_upper);
  bus_.Read(address_lower, &lower);
  bus_.Read(address_upper, &upper);
  *operand = (upper << 8) | lower;
  return true;
}

bool Addressing::Indirect(Operation op, int cycle, uint16_t* operand) {
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
