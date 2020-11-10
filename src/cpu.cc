#include "cpu.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace {

std::string IntToHexString(int num) {
  std::stringstream ss;
  ss << "0x" << std::setfill('0') << std::setw(2) << std::hex << num;
  return ss.str();
}

}  // namespace

class Cpu::Instruction {
 public:
  Instruction(Cpu* cpu) : cpu_(cpu) {}
  Instruction(Instruction&& other) : cpu_(other.cpu_) {}
  virtual ~Instruction() {}
  virtual void Execute(uint8_t opcode) = 0;

 protected:
  std::vector<uint8_t> FetchOperands(int num) {
    std::vector<uint8_t> operands(num);
    for (int i = 0; i < num; ++i) {
      auto status = cpu_->Fetch(cpu_->pc_, &operands[i]);
      if (status != Status::OK) {
        std::cout << "Failed to fetch operands" << std::endl;
        return {};
      }
    }
    cpu_->pc_ += num;
    return operands;
  }

  uint8_t ReadMemoryAtOffset(uint16_t offset) {
    uint8_t value;
    auto status = cpu_->mem_.Read(offset, &value);
    if (status != Memory::Status::OK) {
      std::cout << "Failed to read memory at offset: " << offset << std::endl;
      return 0;
    }
    return value;
  }

  StatusRegister GetStatusRegister() { return cpu_->status_; }

  void SetStatusRegister(StatusRegister status) { cpu_->status_ = status; }

  void SetAcc(uint16_t val) { cpu_->acc_ = val; }

 private:
  // We do not want the instructions to be able to directly access the CPU,
  // because then the implementation of each instruction is strongly coupled to
  // the implementation of the CPU.
  Cpu* cpu_;
};

// No Operation
class NOP : public Cpu::Instruction {
 public:
  using Instruction::Instruction;
  void Execute(uint8_t opcode) override {}
};

// Set Interrupt Disable Status
class SEI : public Cpu::Instruction {
 public:
  using Instruction::Instruction;
  void Execute(uint8_t opcode) override {
    auto s = GetStatusRegister();
    s.int_disable = true;
    SetStatusRegister(s);
  }
};

// Clear Decimal Mode
class CLD : public Cpu::Instruction {
 public:
  using Instruction::Instruction;
  void Execute(uint8_t opcode) override {
    auto s = GetStatusRegister();
    s.bcd_mode = false;
    SetStatusRegister(s);
  }
};

// Load Accumulator
class LDA : public Cpu::Instruction {
 public:
  using Instruction::Instruction;
  void Execute(uint8_t opcode) override {
    // Fetch the number of operands.
    std::vector<uint8_t> operands;
    switch (opcode) {
      case 0xA9:
      case 0xA5:
      case 0xB5:
      case 0xA1:
      case 0xB1:
        operands = FetchOperands(1);
        break;
      case 0xAD:
      case 0xBD:
      case 0xB9:
        operands = FetchOperands(2);
        break;
      default:
        std::cout << "Unsupported LDA variant: " << opcode << std::endl;
        return;
    }

    // Grab the data.
    uint16_t data = 0;
    switch (opcode) {
      case 0xA9:
        data = operands[0];
        break;
      case 0xA5:
        data = ReadMemoryAtOffset(operands[0]);
        break;
      case 0xB5:
      case 0xA1:
      case 0xB1:
      case 0xAD:
      case 0xBD:
      case 0xB9:
      default:
        std::cout << "Unsupported LDA variant: " << opcode << std::endl;
        return;
    }

    // Set the registers.
    SetAcc(data);
    auto s = GetStatusRegister();
    s.zero = (data == 0);
    s.sign = (static_cast<int8_t>(data) < 0);
    SetStatusRegister(s);
  }
};

Cpu::Cpu(Memory& mem) : mem_(mem) {
  // Register all the instructions
  RegisterInstruction<NOP>(0xEA);
  RegisterInstruction<SEI>(0x78);
  RegisterInstruction<CLD>(0xD8);
  RegisterInstruction<LDA>({0xA9, 0xA5, 0xB5, 0xA1, 0xB1, 0xAD, 0xBD, 0xB9});

  // Read the start address
  uint8_t start_address_low, start_address_high;
  Memory::Status status_low, status_high;
  status_low = mem_.Read(0xFFFC, &start_address_low);
  status_high = mem_.Read(0xFFFD, &start_address_high);
  if (status_low != Memory::Status::OK || status_high != Memory::Status::OK) {
    std::cout << "Failed to read the start address" << std::endl;
    return;
  }
  pc_ = (start_address_high << 8) | start_address_low;
  std::cout << "Start address: " << IntToHexString(pc_) << std::endl;
}

Cpu::~Cpu() = default;

Cpu::Status Cpu::Run() {
  Cpu::Status status = Status::OK;

  // Fetch
  uint8_t opcode;
  status = Fetch(pc_, &opcode);
  if (status != Status::OK) return status;
  pc_++;

  // Decode
  auto instruction_it = instructions_.find(opcode);
  if (instruction_it == instructions_.end()) {
    std::cout << "Failed to decode: unknown instruction" << std::endl;
    return Status::UNKNOWN_INSTRUCTION;
  }

  // Execute
  instruction_it->second->Execute(opcode);

  return status;
}

template <class INS>
void Cpu::RegisterInstruction(std::vector<uint8_t> opcodes) {
  auto instruction = std::make_shared<INS>(this);
  for (auto opcode : opcodes) {
    instructions_[opcode] = instruction;
  }
}

template <class INS>
void Cpu::RegisterInstruction(uint8_t opcode) {
  instructions_[opcode] = std::make_shared<INS>(this);
}

Cpu::Status Cpu::Fetch(uint16_t location, uint8_t* opcode) {
  auto status = mem_.Read(location, opcode);
  if (status != Memory::Status::OK) {
    std::cout << "Encountered error "
              << "(" << std::to_string(static_cast<uint8_t>(status)) << ") "
              << "while reading memory at address: " << IntToHexString(location)
              << std::endl;
    return Status::SEGFAULT;
  }

  // Print the address and instruction.
  // TODO: Provide a --verbose flag to disable this at runtime.
  std::cout << "(" << IntToHexString(location)
            << "): " << IntToHexString(*opcode) << std::endl;
  return Status::OK;
}
