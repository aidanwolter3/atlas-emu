#include "src/cpu.h"

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

Cpu::Cpu(Memory& mem, Registers& reg) : mem_(mem), reg_(reg) {
  // Read the start address
  uint8_t start_address_low, start_address_high;
  Memory::Status status_low, status_high;
  status_low = mem_.Read(0xFFFC, &start_address_low);
  status_high = mem_.Read(0xFFFD, &start_address_high);
  if (status_low != Memory::Status::OK || status_high != Memory::Status::OK) {
    std::cout << "Failed to read the start address" << std::endl;
    return;
  }
  reg_.pc = (start_address_high << 8) | start_address_low;
  std::cout << "Start address: " << IntToHexString(reg_.pc) << std::endl;
}

Cpu::~Cpu() = default;

void Cpu::RegisterInstruction(std::unique_ptr<Instruction> instruction,
                              std::vector<uint8_t> opcodes) {
  instructions_.push_back(std::move(instruction));
  Instruction* instruction_ptr = instructions_.back().get();
  for (auto opcode : opcodes) {
    instruction_map_[opcode] = instruction_ptr;
  }
}

Cpu::Status Cpu::Run() {
  Cpu::Status status = Status::OK;

  // Fetch
  uint8_t opcode;
  status = Fetch(reg_.pc, &opcode);
  if (status != Status::OK) return status;
  reg_.pc++;

  // Decode
  auto instruction_it = instruction_map_.find(opcode);
  if (instruction_it == instruction_map_.end()) {
    std::cout << "Failed to decode: unknown instruction" << std::endl;
    return Status::UNKNOWN_INSTRUCTION;
  }

  // Execute
  Instruction* instruction_ptr = instruction_it->second;
  instruction_ptr->Execute(opcode);

  return status;
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
