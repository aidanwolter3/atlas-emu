#include "src/engine/instruction/addressing.h"

#include "src/engine/base/log.h"

Addressing::Addressing(Bus& bus, Registers& reg) : bus_(bus), reg_(reg) {
  ResetExecuteData();
}

bool Addressing::Execute(Instruction::Config& config, int cycle) {
  if (cycle < 2) return false;

  if (cycle == 2) {
    uint16_t initial_pc = reg_.pc - 1;
    bool success = true;

    // Based on the mode, fetch the operand/address/data.
    success = FetchExecuteData(config);
    if (!success) return true;

    // If needed, read the data from the operand/address.
    success = MaybeReadData(config);
    if (!success) return true;

    // Execute the instruction.
    Instruction::ExecuteResult execute_result =
        config.instruction->Execute(data_.data.value_or(0));
    data_.cycles += execute_result.cycles;

    // If needed, write the data back.
    success = MaybeWriteData(execute_result.data, config);
    if (!success) return true;

    // Log the instruction.
    LOG(DEBUG) << Log::Hex(initial_pc) << "(" << data_.cycles << ")"
               << ": " << config.instruction->GetLogName() << " "
               << ConstructOperandLog(config);
  }

  // Wait for the appropriate amount of cycles.
  if (cycle < data_.cycles) return false;

  ResetExecuteData();
  return true;
}

void Addressing::ResetExecuteData() {
  data_.operand.reset();
  data_.address.reset();
  data_.data.reset();
  data_.cycles = 2;
}

bool Addressing::FetchExecuteData(Instruction::Config& config) {
  switch (config.mode) {
    case Instruction::Mode::kImplied:
      Implied(config.operation);
      return true;
    case Instruction::Mode::kImmediate:
      Immediate(config.operation);
      return true;
    case Instruction::Mode::kImmediateAddress:
      ImmediateAddress(config.operation);
      return true;
    case Instruction::Mode::kZeroPage:
      ZeroPage(config.operation);
      return true;
    case Instruction::Mode::kZeroPageX:
      ZeroPageX(config.operation);
      return true;
    case Instruction::Mode::kZeroPageY:
      ZeroPageY(config.operation);
      return true;
    case Instruction::Mode::kAbsolute:
      Absolute(config.operation);
      return true;
    case Instruction::Mode::kAbsoluteX:
      AbsoluteX(config.operation);
      return true;
    case Instruction::Mode::kAbsoluteY:
      AbsoluteY(config.operation);
      return true;
    case Instruction::Mode::kIndirect:
      Indirect(config.operation);
      return true;
    case Instruction::Mode::kIndirectX:
      IndirectX(config.operation);
      return true;
    case Instruction::Mode::kIndirectY:
      IndirectY(config.operation);
      return true;
    default:
      LOG(ERROR) << "Invalid addressing mode: "
                 << static_cast<int>(config.mode);
      return false;
  }
}

bool Addressing::MaybeReadData(Instruction::Config& config) {
  // Only read if we do not have |data| already.
  if (data_.data.has_value()) {
    return true;
  }

  if (config.operation != Instruction::Operation::kRead &&
      config.operation != Instruction::Operation::kReadWrite) {
    return true;
  }

  // Implied instructions get the ACC.
  if (config.mode == Instruction::Mode::kImplied) {
    data_.data = reg_.acc;
    return true;
  }

  if (!data_.address.has_value()) {
    LOG(ERROR) << "Address is not set!";
    return false;
  }

  uint8_t data;
  bus_.Read(data_.address.value(), &data);
  data_.data = data;
  return true;
}

bool Addressing::MaybeWriteData(uint8_t data, Instruction::Config& config) {
  if (config.operation != Instruction::Operation::kWrite &&
      config.operation != Instruction::Operation::kReadWrite) {
    return true;
  }

  // Implied instructions use the ACC.
  if (config.mode == Instruction::Mode::kImplied) {
    reg_.acc = data;
    return true;
  }

  if (!data_.address.has_value()) {
    LOG(ERROR) << "Address is not set!";
    return false;
  }

  bus_.Write(data_.address.value(), data);
  return true;
}

std::string Addressing::ConstructOperandLog(Instruction::Config& config) {
  if (!data_.operand.has_value()) {
    return "";
  }

  switch (config.mode) {
    case Instruction::Mode::kImmediate:
      return "#" + Log::Hex(data_.operand.value());
    case Instruction::Mode::kImmediateAddress:
    case Instruction::Mode::kZeroPage:
    case Instruction::Mode::kAbsolute:
    case Instruction::Mode::kIndirect:
      return "$" + Log::Hex(data_.operand.value());
    case Instruction::Mode::kZeroPageX:
    case Instruction::Mode::kAbsoluteX:
      return "$" + Log::Hex(data_.operand.value()) + ", X";
    case Instruction::Mode::kZeroPageY:
    case Instruction::Mode::kAbsoluteY:
      return "$" + Log::Hex(data_.operand.value()) + ", Y";
    case Instruction::Mode::kIndirectX:
      return "(" + Log::Hex(data_.operand.value()) + ", X)";
    case Instruction::Mode::kIndirectY:
      return "(" + Log::Hex(data_.operand.value()) + "), Y";
    default:
      LOG(ERROR) << "Invalid addressing mode: "
                 << static_cast<int>(config.mode);
      return "";
  }
}

void Addressing::Implied(Instruction::Operation operation) { data_.cycles = 2; }

void Addressing::Immediate(Instruction::Operation operation) {
  uint8_t operand;
  bus_.Read(reg_.pc++, &operand);

  data_.operand = operand;
  data_.data = data_.operand;
  data_.cycles = 2;
}

void Addressing::ImmediateAddress(Instruction::Operation operation) {
  uint8_t address_lower, address_upper;
  bus_.Read(reg_.pc, &address_lower);
  bus_.Read(reg_.pc + 1, &address_upper);
  reg_.pc += 2;

  data_.operand = (address_upper << 8) | address_lower;
  data_.data = data_.operand;
  data_.cycles = 3;
}

void Addressing::ZeroPage(Instruction::Operation operation) {
  uint8_t address;
  bus_.Read(reg_.pc, &address);
  reg_.pc++;

  data_.operand = address;
  data_.address = address;
  data_.cycles = 3;
  if (operation == Instruction::Operation::kReadWrite) {
    data_.cycles = 5;
  }
}

void Addressing::ZeroPageX(Instruction::Operation operation) {
  uint8_t address;
  bus_.Read(reg_.pc, &address);
  reg_.pc++;

  data_.operand = address;
  data_.address = address + reg_.x;
  data_.cycles = 4;
  if (operation == Instruction::Operation::kReadWrite) {
    data_.cycles = 6;
  }
}

void Addressing::ZeroPageY(Instruction::Operation operation) {
  uint8_t address;
  bus_.Read(reg_.pc, &address);
  reg_.pc++;

  data_.operand = address;
  data_.address = address + reg_.y;
  data_.cycles = 4;
  if (operation == Instruction::Operation::kReadWrite) {
    data_.cycles = 6;
  }
}

void Addressing::Absolute(Instruction::Operation operation) {
  uint8_t address_lower, address_upper;
  bus_.Read(reg_.pc, &address_lower);
  bus_.Read(reg_.pc + 1, &address_upper);
  uint16_t address = (address_upper << 8) | address_lower;
  reg_.pc += 2;

  data_.operand = address;
  data_.address = address;
  data_.cycles = 4;
  if (operation == Instruction::Operation::kReadWrite) {
    data_.cycles = 6;
  }
}

void Addressing::AbsoluteX(Instruction::Operation operation) {
  uint8_t address_lower, address_upper;
  bus_.Read(reg_.pc, &address_lower);
  bus_.Read(reg_.pc + 1, &address_upper);
  uint16_t address = ((address_upper << 8) | address_lower);
  reg_.pc += 2;

  data_.operand = address;
  data_.address = address + reg_.x;
  data_.cycles = 5;  // TODO: This is sometimes 4 for Read operations.
  if (operation == Instruction::Operation::kReadWrite) {
    data_.cycles = 7;
  }
}

void Addressing::AbsoluteY(Instruction::Operation operation) {
  uint8_t address_lower, address_upper;
  bus_.Read(reg_.pc, &address_lower);
  bus_.Read(reg_.pc + 1, &address_upper);
  uint16_t address = ((address_upper << 8) | address_lower);
  reg_.pc += 2;

  data_.operand = address;
  data_.address = address + reg_.y;
  data_.cycles = 5;  // TODO: This is sometimes 4 for Read operations.
  if (operation == Instruction::Operation::kReadWrite) {
    data_.cycles = 7;
  }
}

void Addressing::Indirect(Instruction::Operation operation) {
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

  data_.operand = data;
  data_.data = data;
  data_.cycles = 5;
}

void Addressing::IndirectX(Instruction::Operation operation) {
  uint8_t operand;
  bus_.Read(reg_.pc, &operand);
  reg_.pc++;

  uint8_t lower, upper;
  bus_.Read(operand + reg_.x, &lower);
  bus_.Read(operand + reg_.x + 1, &upper);
  uint16_t address = (upper << 8) | lower;

  data_.operand = operand;
  data_.address = address;
  data_.cycles = 6;
  if (operation == Instruction::Operation::kReadWrite) {
    data_.cycles = 8;
  }
}

void Addressing::IndirectY(Instruction::Operation operation) {
  uint8_t operand;
  bus_.Read(reg_.pc, &operand);
  reg_.pc++;

  uint8_t lower, upper;
  bus_.Read(operand, &lower);
  bus_.Read(operand + 1, &upper);
  uint16_t address = ((upper << 8) | lower) + reg_.y;

  data_.operand = operand;
  data_.address = address;
  data_.cycles = 6;  // TODO: This is sometimes 5 for Read operations.
  if (operation == Instruction::Operation::kReadWrite) {
    data_.cycles = 8;
  }
}
