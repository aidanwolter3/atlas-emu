#include "src/instruction/instruction.h"

#include <iostream>

std::vector<uint8_t> Instruction::FetchOperands(int num) {
  std::vector<uint8_t> operands(num);
  uint16_t pc = cpu_.GetPc();
  for (int i = 0; i < num; ++i) {
    auto status = mem_.Read(pc++, &operands[i]);
    if (status != Memory::Status::OK) {
      std::cout << "Failed to fetch operands" << std::endl;
      return {};
    }
  }
  cpu_.SetPc(pc);
  return operands;
}
