#include "src/engine/instruction/addressing.h"

#include <iostream>

Addressing::Addressing(Bus& bus, Registers& reg) : bus_(bus), reg_(reg) {}

bool Addressing::Execute(Instruction::Config& config, int cycle) {
  if (cycle < 2) return false;

  // Execute the instruction.
  if (cycle == 2) {
    // Get the address of interest, and in some cases, the operand.
    Addressing::Result result;
    operand_ = 0;
    switch (config.mode) {
      case Instruction::Mode::kImplied:
        result.cycles = 2;
        break;
      case Instruction::Mode::kImmediate:
        result = Immediate(config.operation);
        address_ = result.data;
        break;
      case Instruction::Mode::kImmediateAddress:
        result = ImmediateAddress(config.operation);
        operand_ = result.data;
        break;
      case Instruction::Mode::kZeroPage:
        result = ZeroPage(config.operation);
        address_ = result.data;
        break;
      case Instruction::Mode::kZeroPageX:
        result = ZeroPageX(config.operation);
        address_ = result.data;
        break;
      case Instruction::Mode::kZeroPageY:
        result = ZeroPageY(config.operation);
        address_ = result.data;
        break;
      case Instruction::Mode::kAbsolute:
        result = Absolute(config.operation);
        address_ = result.data;
        break;
      case Instruction::Mode::kAbsoluteX:
        result = AbsoluteX(config.operation);
        address_ = result.data;
        break;
      case Instruction::Mode::kAbsoluteY:
        result = AbsoluteY(config.operation);
        address_ = result.data;
        break;
      case Instruction::Mode::kIndirect:
        result = Indirect(config.operation);
        operand_ = result.data;
        break;
      case Instruction::Mode::kIndirectX:
        result = IndirectX(config.operation);
        address_ = result.data;
        break;
      case Instruction::Mode::kIndirectY:
        result = IndirectY(config.operation);
        address_ = result.data;
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
    if (config.operation == Instruction::Operation::kRead ||
        config.operation == Instruction::Operation::kReadWrite) {
      if (config.mode == Instruction::Mode::kImplied) {
        operand_ = reg_.acc;
      } else {
        if (!address_.has_value()) {
          std::cout << "Address is not set!" << std::endl;
          return true;
        }
        uint8_t byte;
        bus_.Read(*address_, &byte);
        operand_ = byte;
      }
    }

    // Run the instruction with the operand.
    uint8_t data_to_write = config.instruction->Execute(*operand_);

    // Write the operand on the last cycle (optional).
    if (config.operation == Instruction::Operation::kWrite ||
        config.operation == Instruction::Operation::kReadWrite) {
      if (config.mode == Instruction::Mode::kImplied) {
        reg_.acc = data_to_write;
      } else {
        if (!address_.has_value()) {
          std::cout << "Address is not set!" << std::endl;
          return true;
        }

        bus_.Write(*address_, data_to_write);
      }
    }
  }

  // Wait for the appropriate amount of cycles.
  if (cycle < cycles_) return false;

  address_.reset();
  operand_.reset();
  cycles_ = 0;
  return true;
}

Addressing::Result Addressing::Immediate(Instruction::Operation operation) {
  return {
      .data = reg_.pc++,
      .cycles = 2,
  };
}

Addressing::Result Addressing::ImmediateAddress(
    Instruction::Operation operation) {
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

Addressing::Result Addressing::ZeroPage(Instruction::Operation operation) {
  uint8_t address;
  bus_.Read(reg_.pc, &address);
  uint16_t data = address;
  reg_.pc++;

  int cycles = 3;
  if (operation == Instruction::Operation::kReadWrite) {
    cycles = 5;
  }

  return {
      .data = data,
      .cycles = cycles,
  };
}

Addressing::Result Addressing::ZeroPageX(Instruction::Operation operation) {
  uint8_t address;
  bus_.Read(reg_.pc, &address);
  uint16_t data = address + reg_.x;
  reg_.pc++;

  int cycles = 4;
  if (operation == Instruction::Operation::kReadWrite) {
    cycles = 6;
  }

  return {
      .data = data,
      .cycles = cycles,
  };
}

Addressing::Result Addressing::ZeroPageY(Instruction::Operation operation) {
  uint8_t address;
  bus_.Read(reg_.pc, &address);
  uint16_t data = address + reg_.y;
  reg_.pc++;

  int cycles = 4;
  if (operation == Instruction::Operation::kReadWrite) {
    cycles = 6;
  }

  return {
      .data = data,
      .cycles = cycles,
  };
}

Addressing::Result Addressing::Absolute(Instruction::Operation operation) {
  uint8_t address_lower, address_upper;
  bus_.Read(reg_.pc, &address_lower);
  bus_.Read(reg_.pc + 1, &address_upper);
  uint16_t data = (address_upper << 8) | address_lower;
  reg_.pc += 2;

  int cycles = 4;
  if (operation == Instruction::Operation::kReadWrite) {
    cycles = 6;
  }

  return {
      .data = data,
      .cycles = cycles,
  };
}

Addressing::Result Addressing::AbsoluteX(Instruction::Operation operation) {
  uint8_t address_lower, address_upper;
  bus_.Read(reg_.pc, &address_lower);
  bus_.Read(reg_.pc + 1, &address_upper);
  uint16_t data = ((address_upper << 8) | address_lower) + reg_.x;
  reg_.pc += 2;

  int cycles = 5;  // TODO: This is sometimes 4 for Read operations.
  if (operation == Instruction::Operation::kReadWrite) {
    cycles = 7;
  }

  return {
      .data = data,
      .cycles = cycles,
  };
}

Addressing::Result Addressing::AbsoluteY(Instruction::Operation operation) {
  uint8_t address_lower, address_upper;
  bus_.Read(reg_.pc, &address_lower);
  bus_.Read(reg_.pc + 1, &address_upper);
  uint16_t data = ((address_upper << 8) | address_lower) + reg_.y;
  reg_.pc += 2;

  int cycles = 5;  // TODO: This is sometimes 4 for Read operations.
  if (operation == Instruction::Operation::kReadWrite) {
    cycles = 7;
  }

  return {
      .data = data,
      .cycles = cycles,
  };
}

Addressing::Result Addressing::Indirect(Instruction::Operation operation) {
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

Addressing::Result Addressing::IndirectX(Instruction::Operation operation) {
  uint8_t address;
  bus_.Read(reg_.pc, &address);
  reg_.pc++;

  uint8_t lower, upper;
  bus_.Read(address + reg_.x, &lower);
  bus_.Read(address + reg_.x + 1, &upper);
  uint16_t data = (upper << 8) | lower;

  int cycles = 6;
  if (operation == Instruction::Operation::kReadWrite) {
    cycles = 8;
  }

  return {
      .data = data,
      .cycles = cycles,
  };
}

Addressing::Result Addressing::IndirectY(Instruction::Operation operation) {
  uint8_t address;
  bus_.Read(reg_.pc, &address);
  reg_.pc++;

  uint8_t lower, upper;
  bus_.Read(address, &lower);
  bus_.Read(address + 1, &upper);
  uint16_t data = ((upper << 8) | lower) + reg_.y;

  int cycles = 6;  // TODO: This is sometimes 5 for Read operations.
  if (operation == Instruction::Operation::kReadWrite) {
    cycles = 8;
  }

  return {
      .data = data,
      .cycles = cycles,
  };
}
