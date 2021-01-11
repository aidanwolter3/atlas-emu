#include "src/engine/instruction/addressing.h"

#include <iostream>

Addressing::Addressing(Bus& bus, Registers& reg) : bus_(bus), reg_(reg) {}

bool Addressing::Execute(Instruction2::Config& config, int cycle) {
  // Get the address of interest, and in some cases, the operand.
  if (cycle == 2) {
    switch (config.mode) {
      case Instruction2::Mode::kImplied:
        break;
      case Instruction2::Mode::kImmediate:
        address_ = Immediate();
        break;
      case Instruction2::Mode::kImmediateAddress:
        operand_ = ImmediateAddress();
        break;
      case Instruction2::Mode::kZeroPage:
        address_ = ZeroPage();
        break;
      case Instruction2::Mode::kAbsolute:
        address_ = Absolute();
        break;
      case Instruction2::Mode::kIndirect:
        operand_ = Indirect();
        break;
      default:
        std::cout << "Invalid addressing mode: " << (int)config.mode
                  << std::endl;
        return true;
    }
  }

  // Read the operand on the post-opcode first cycle (optional).
  // For ImmediateAddress/Indirect we do not read, as the operand _is_ the
  // address.
  if (cycle == 2 && address_ &&
      (config.operation == Instruction2::Operation::kRead ||
       config.operation == Instruction2::Operation::kReadWrite)) {
    uint8_t byte;
    bus_.Read(*address_, &byte);
    operand_ = byte;
  }

  // Run the instruction with the operand.
  // The instruction dictates how many cycles to consume.
  std::optional<uint8_t> result =
      config.instruction->Execute(config.opcode, config.mode, operand_, cycle);
  if (!result) return false;

  // Write the operand on the last cycle (optional).
  if (address_ && (config.operation == Instruction2::Operation::kWrite ||
                   config.operation == Instruction2::Operation::kReadWrite)) {
    bus_.Write(*address_, *result);
  }

  operand_ = 0;
  address_.reset();
  return true;
}

uint16_t Addressing::Immediate() { return reg_.pc; }

uint16_t Addressing::ImmediateAddress() {
  uint8_t address_lower, address_upper;
  bus_.Read(reg_.pc, &address_lower);
  bus_.Read(reg_.pc + 1, &address_upper);
  return (address_upper << 8) | address_lower;
}

uint16_t Addressing::ZeroPage() {
  uint8_t address;
  bus_.Read(reg_.pc, &address);
  return address;
}

uint16_t Addressing::Absolute() {
  uint8_t address_lower, address_upper;
  bus_.Read(reg_.pc, &address_lower);
  bus_.Read(reg_.pc + 1, &address_upper);
  return (address_upper << 8) | address_lower;
}

uint16_t Addressing::Indirect() {
  uint8_t lower, upper;
  bus_.Read(reg_.pc, &lower);
  bus_.Read(reg_.pc + 1, &upper);

  uint16_t address_location_1 = ((upper << 8) | lower);
  uint16_t address_location_2 =
      ((upper << 8) | static_cast<uint8_t>(lower + 1));

  bus_.Read(address_location_1, &lower);
  bus_.Read(address_location_2, &upper);
  return (upper << 8) | lower;
}
