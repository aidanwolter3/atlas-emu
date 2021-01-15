#include "src/engine/instruction/addressing.h"

#include <iostream>

Addressing::Addressing(Bus& bus, Registers& reg) : bus_(bus), reg_(reg) {}

bool Addressing::Execute(Instruction2::Config& config, int cycle) {
  if (cycle < 2) return false;

  // Execute the instruction.
  if (cycle == 2) {
    // Get the address of interest, and in some cases, the operand.
    Addressing::Result result = {
        .data = 0,
        .cycles = 0,
    };
    switch (config.mode) {
      case Instruction2::Mode::kImplied:
        operand_ = 0;
        break;
      case Instruction2::Mode::kImmediate:
        result = Immediate(config.operation);
        address_ = result.data;
        break;
      case Instruction2::Mode::kImmediateAddress:
        result = ImmediateAddress(config.operation);
        operand_ = result.data;
        break;
      case Instruction2::Mode::kZeroPage:
        result = ZeroPage(config.operation);
        address_ = result.data;
        break;
      case Instruction2::Mode::kZeroPageX:
        result = ZeroPageX(config.operation);
        address_ = result.data;
        break;
      case Instruction2::Mode::kZeroPageY:
        result = ZeroPageY(config.operation);
        address_ = result.data;
        break;
      case Instruction2::Mode::kAbsolute:
        result = Absolute(config.operation);
        address_ = result.data;
        break;
      case Instruction2::Mode::kAbsoluteX:
        result = AbsoluteX(config.operation);
        address_ = result.data;
        break;
      case Instruction2::Mode::kAbsoluteY:
        result = AbsoluteY(config.operation);
        address_ = result.data;
        break;
      case Instruction2::Mode::kIndirect:
        result = Indirect(config.operation);
        operand_ = result.data;
        break;
      case Instruction2::Mode::kIndirectX:
        result = IndirectX(config.operation);
        operand_ = result.data;
        break;
      case Instruction2::Mode::kIndirectY:
        result = IndirectY(config.operation);
        operand_ = result.data;
        break;
      default:
        std::cout << "Invalid addressing mode: " << (int)config.mode
                  << std::endl;
        return true;
    }
    cycles_ = result.cycles;

    // Read the operand on the post-opcode first cycle (optional).
    // For ImmediateAddress/Indirect we do not read, as the operand _is_ the
    // address.
    if (config.operation == Instruction2::Operation::kRead ||
        config.operation == Instruction2::Operation::kReadWrite) {
      if (!address_) {
        std::cout << "Address is not set!" << std::endl;
        return true;
      }
      uint8_t byte;
      bus_.Read(*address_, &byte);
      operand_ = byte;
    }

    if (!operand_) {
      std::cout << "Operand is not set!" << std::endl;
      return true;
    }

    // Run the instruction with the operand.
    uint8_t data_to_write =
        config.instruction->Execute(config.opcode, *operand_);

    // Write the operand on the last cycle (optional).
    if (address_ && (config.operation == Instruction2::Operation::kWrite ||
                     config.operation == Instruction2::Operation::kReadWrite)) {
      bus_.Write(*address_, data_to_write);
    }
  }

  // Wait for the appropriate amount of cycles.
  if (cycle < cycles_) return false;

  address_.reset();
  operand_.reset();
  cycles_ = 0;
  return true;
}

Addressing::Result Addressing::Immediate(Instruction2::Operation operation) {
  return {
      .data = reg_.pc++,
      .cycles = 2,
  };
}

Addressing::Result Addressing::ImmediateAddress(
    Instruction2::Operation operation) {
  uint8_t address_lower, address_upper;
  bus_.Read(reg_.pc, &address_lower);
  bus_.Read(reg_.pc + 1, &address_upper);
  uint16_t data = (address_upper << 8) | address_lower;
  reg_.pc += 2;

  return {
      .data = data,
      .cycles = 3,
  };
}

Addressing::Result Addressing::ZeroPage(Instruction2::Operation operation) {
  uint8_t address;
  bus_.Read(reg_.pc, &address);
  uint16_t data = address;
  reg_.pc++;

  int cycles = 3;
  if (operation == Instruction2::Operation::kReadWrite) {
    cycles = 5;
  }

  return {
      .data = data,
      .cycles = cycles,
  };
}

Addressing::Result Addressing::ZeroPageX(Instruction2::Operation operation) {
  uint8_t address;
  bus_.Read(reg_.pc, &address);
  uint16_t data = address + reg_.x;
  reg_.pc++;

  int cycles = 4;
  if (operation == Instruction2::Operation::kReadWrite) {
    cycles = 6;
  }

  return {
      .data = data,
      .cycles = cycles,
  };
}

Addressing::Result Addressing::ZeroPageY(Instruction2::Operation operation) {
  uint8_t address;
  bus_.Read(reg_.pc, &address);
  uint16_t data = address + reg_.y;
  reg_.pc++;

  int cycles = 4;
  if (operation == Instruction2::Operation::kReadWrite) {
    cycles = 6;
  }

  return {
      .data = data,
      .cycles = cycles,
  };
}

Addressing::Result Addressing::Absolute(Instruction2::Operation operation) {
  uint8_t address_lower, address_upper;
  bus_.Read(reg_.pc, &address_lower);
  bus_.Read(reg_.pc + 1, &address_upper);
  uint16_t data = (address_upper << 8) | address_lower;
  reg_.pc += 2;

  int cycles = 4;
  if (operation == Instruction2::Operation::kReadWrite) {
    cycles = 6;
  }

  return {
      .data = data,
      .cycles = cycles,
  };
}

Addressing::Result Addressing::AbsoluteX(Instruction2::Operation operation) {
  uint8_t address_lower, address_upper;
  bus_.Read(reg_.pc, &address_lower);
  bus_.Read(reg_.pc + 1, &address_upper);
  uint16_t data = ((address_upper << 8) | address_lower) + reg_.x;
  reg_.pc += 2;

  int cycles = 5;  // TODO: This is sometimes 4 for Read operations.
  if (operation == Instruction2::Operation::kReadWrite) {
    cycles = 7;
  }

  return {
      .data = data,
      .cycles = cycles,
  };
}

Addressing::Result Addressing::AbsoluteY(Instruction2::Operation operation) {
  uint8_t address_lower, address_upper;
  bus_.Read(reg_.pc, &address_lower);
  bus_.Read(reg_.pc + 1, &address_upper);
  uint16_t data = ((address_upper << 8) | address_lower) + reg_.y;
  reg_.pc += 2;

  int cycles = 5;  // TODO: This is sometimes 4 for Read operations.
  if (operation == Instruction2::Operation::kReadWrite) {
    cycles = 7;
  }

  return {
      .data = data,
      .cycles = cycles,
  };
}

Addressing::Result Addressing::Indirect(Instruction2::Operation operation) {
  uint8_t lower, upper;
  bus_.Read(reg_.pc, &lower);
  bus_.Read(reg_.pc + 1, &upper);
  reg_.pc += 2;

  uint16_t address_location_1 = ((upper << 8) | lower);
  uint16_t address_location_2 =
      ((upper << 8) | static_cast<uint8_t>(lower + 1));

  bus_.Read(address_location_1, &lower);
  bus_.Read(address_location_2, &upper);
  uint16_t data = (upper << 8) | lower;

  return {
      .data = data,
      .cycles = 5,
  };
}

Addressing::Result Addressing::IndirectX(Instruction2::Operation operation) {
  uint8_t address;
  bus_.Read(reg_.pc, &address);
  reg_.pc++;

  uint8_t lower, upper;
  bus_.Read(address + reg_.x, &lower);
  bus_.Read(address + reg_.x + 1, &upper);
  uint16_t data = (upper << 8) | lower;

  int cycles = 6;
  if (operation == Instruction2::Operation::kReadWrite) {
    cycles = 8;
  }

  return {
      .data = data,
      .cycles = cycles,
  };
}

Addressing::Result Addressing::IndirectY(Instruction2::Operation operation) {
  uint8_t address;
  bus_.Read(reg_.pc, &address);
  reg_.pc++;

  uint8_t lower, upper;
  bus_.Read(address, &lower);
  bus_.Read(address + 1, &upper);
  uint16_t data = ((upper << 8) | lower) + reg_.y;

  int cycles = 6;  // TODO: This is sometimes 5 for Read operations.
  if (operation == Instruction2::Operation::kReadWrite) {
    cycles = 8;
  }

  return {
      .data = data,
      .cycles = cycles,
  };
}
