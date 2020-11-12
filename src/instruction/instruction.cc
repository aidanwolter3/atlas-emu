#include "src/public/instruction.h"

#include <iostream>

std::vector<uint8_t> Instruction::FetchOperands(int num) {
  std::vector<uint8_t> operands(num);
  uint16_t pc = reg_.pc;
  for (int i = 0; i < num; ++i) {
    auto status = mem_.Read(pc++, &operands[i]);
    if (status != Memory::Status::OK) {
      std::cout << "Failed to fetch operands" << std::endl;
      return {};
    }
  }
  reg_.pc = pc;
  return operands;
}

uint16_t Instruction::Immediate() {
  std::vector<uint8_t> operands = FetchOperands(1);
  return operands[0];
}

uint16_t Instruction::ZeroPage() { return IndexedZeroPage(0); }

uint16_t Instruction::IndexedZeroPage(uint8_t index) {
  std::vector<uint8_t> operands = FetchOperands(1);
  return operands[0] + index;
}

uint16_t Instruction::Absolute() { return IndexedAbsolute(0); }

uint16_t Instruction::IndexedAbsolute(uint8_t index) {
  std::vector<uint8_t> operands = FetchOperands(2);
  return ((operands[1] << 8) | operands[0]) + index;
}

uint16_t Instruction::IndexedIndirect(uint8_t index) {
  std::vector<uint8_t> operands = FetchOperands(1);
  uint8_t address;
  mem_.Read(operands[0] + index, &address);
  return address;
}

uint16_t Instruction::IndirectIndexed(uint8_t index) {
  std::vector<uint8_t> operands = FetchOperands(1);
  uint8_t address;
  mem_.Read(operands[0], &address);
  return address + index;
}
