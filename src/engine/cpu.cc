#include "src/engine/cpu.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "src/engine/public/constants.h"

namespace {

std::string IntToHexString(int num) {
  std::stringstream ss;
  ss << "0x" << std::setfill('0') << std::setw(2) << std::hex << num;
  return ss.str();
}

}  // namespace

Cpu::Cpu(EventLogger& event_logger, Bus& bus, Registers& reg)
    : event_logger_(event_logger), bus_(bus), reg_(reg) {}

void Cpu::Reset() { reg_.pc = ReadAddressFromVectorTable(0xFFFC); }

void Cpu::NMI() {
  bus_.Write(kStackStartAddress + reg_.sp--, reg_.pc >> 8);
  bus_.Write(kStackStartAddress + reg_.sp--, reg_.pc & 0xFF);
  bus_.Write(kStackStartAddress + reg_.sp--, reg_.status.to_ulong());
  reg_.status.set(Status::kIntDisable);
  reg_.pc = ReadAddressFromVectorTable(0xFFFA);
}

void Cpu::Tick() {
  // Fetch
  uint8_t opcode;
  bus_.Read(reg_.pc, &opcode);
  reg_.pc++;

  // Decode
  auto instruction_it = instruction_map_.find(opcode);
  if (instruction_it == instruction_map_.end()) {
    std::string event_name =
        "Failed to decode: unknown instruction: " + IntToHexString(opcode);
    event_logger_.LogEvent(
        {.type = EventLogger::EventType::kError, .name = event_name});
    return;
  }

  // Execute
  Instruction* instruction_ptr = instruction_it->second;
  instruction_ptr->Execute(opcode);
}

void Cpu::DumpRegisters() {
  std::cout << "-- CPU --" << std::endl;
  std::cout << "PC=" << IntToHexString(reg_.pc) << std::endl;
  std::cout << "SP=" << IntToHexString(reg_.sp) << std::endl;
  std::cout << "Status=" << reg_.status << std::endl;
  std::cout << "ACC=" << IntToHexString(reg_.acc) << std::endl;
  std::cout << "X=" << IntToHexString(reg_.x) << std::endl;
  std::cout << "Y=" << IntToHexString(reg_.y) << std::endl;
  std::cout << "---------" << std::endl;

  // Dump the stack
  std::cout << "-- Stack --" << std::endl;
  for (uint16_t addr = kStackStartAddress + reg_.sp + 1;
       addr < kStackStartAddress + 0x100; ++addr) {
    uint8_t stack_data;
    bus_.Read(addr, &stack_data);
    std::cout << IntToHexString(addr) << ": " << IntToHexString(stack_data)
              << std::endl;
  }
  std::cout << "-----------" << std::endl;
}

void Cpu::RegisterInstruction(std::unique_ptr<Instruction> instruction,
                              std::vector<uint8_t> opcodes) {
  instructions_.push_back(std::move(instruction));
  Instruction* instruction_ptr = instructions_.back().get();
  for (auto opcode : opcodes) {
    instruction_map_[opcode] = instruction_ptr;
  }
}

uint16_t Cpu::ReadAddressFromVectorTable(uint16_t address) {
  uint8_t start_address_low, start_address_high;
  Peripheral::Status status_low, status_high;
  status_low = bus_.Read(address, &start_address_low);
  status_high = bus_.Read(address + 1, &start_address_high);
  if (status_low != Peripheral::Status::OK ||
      status_high != Peripheral::Status::OK) {
    std::string event_name =
        "Failed to read from vector table address: " + IntToHexString(address);
    event_logger_.LogEvent(
        {.type = EventLogger::EventType::kError, .name = event_name});
    return 0;
  }
  return (start_address_high << 8) | start_address_low;
}
