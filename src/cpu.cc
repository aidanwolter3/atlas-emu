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

Cpu::Cpu(Memory& mem) : mem_(mem) {
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
  uint8_t hex_instruction;
  status = Fetch(pc_, &hex_instruction);
  if (status != Status::OK) return status;

  // Decode
  Instruction instruction;
  status = Decode(hex_instruction, &instruction);
  if (status != Status::OK) return status;

  pc_++;
  return status;
}

Cpu::Status Cpu::Fetch(uint16_t location, uint8_t* hex_instruction) {
  auto status = mem_.Read(location, hex_instruction);
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
            << "): " << IntToHexString(*hex_instruction) << std::endl;
  return Status::OK;
}

Cpu::Status Cpu::Decode(uint8_t hex_instruction, Instruction* instruction) {
  switch (hex_instruction) {
    case 0xEA:
      *instruction = NOP();
      return Status::OK;
  }
  std::cout << "Failed to decode: unknown instruction" << std::endl;
  return Status::UNKNOWN_INSTRUCTION;
}
