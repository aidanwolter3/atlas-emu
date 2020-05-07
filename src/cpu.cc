#include "cpu.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

namespace {

std::string IntToHexString(int num) {
  std::stringstream ss;
  ss << "0x" << std::setfill('0') << std::setw(2) << std::hex << num;
  return ss.str();
}

}  // namespace

Cpu::Cpu(Memory& mem) : mem_(mem) {
  stack_.pc = 0;
}

Cpu::~Cpu() = default;

Cpu::Status Cpu::Run() {
  uint8_t instruction;
  auto status = mem_.Read(stack_.pc, &instruction);
  if (status != Memory::Status::OK) {
    std::cout << "Encountered error "
              << "(" << std::to_string(static_cast<uint8_t>(status)) << ") "
              << "while reading memory at address: "
              << IntToHexString(stack_.pc) << std::endl;
    return Status::SEGFAULT;
  }

  // Print the address and instruction.
  std::cout << "(" << IntToHexString(stack_.pc) << "): "
            << IntToHexString(instruction) << std::endl;

  stack_.pc++;
  return Status::OK;
}
