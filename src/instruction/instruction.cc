#include "src/instruction/instruction.h"

#include <cstdint>
#include <iostream>
#include <vector>

#include "src/cpu.h"

Instruction::Instruction(Cpu* cpu) : cpu_(cpu) {}
Instruction::Instruction(Instruction&& other) : cpu_(other.cpu_) {}

std::vector<uint8_t> Instruction::FetchOperands(int num) {
  std::vector<uint8_t> operands(num);
  for (int i = 0; i < num; ++i) {
    auto status = cpu_->Fetch(cpu_->pc_, &operands[i]);
    if (status != Cpu::Status::OK) {
      std::cout << "Failed to fetch operands" << std::endl;
      return {};
    }
  }
  cpu_->pc_ += num;
  return operands;
}

uint8_t Instruction::ReadMemoryAtOffset(uint16_t offset) {
  uint8_t value;
  auto status = cpu_->mem_.Read(offset, &value);
  if (status != Memory::Status::OK) {
    std::cout << "Failed to read memory at offset: " << offset << std::endl;
    return 0;
  }
  return value;
}

StatusRegister Instruction::GetStatusRegister() { return cpu_->status_; }

void Instruction::SetStatusRegister(StatusRegister status) {
  cpu_->status_ = status;
}

void Instruction::SetAcc(uint16_t val) { cpu_->acc_ = val; }
