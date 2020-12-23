#include "src/cpu.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "src/public/constants.h"

namespace {

std::string IntToHexString(int num) {
  std::stringstream ss;
  ss << "0x" << std::setfill('0') << std::setw(2) << std::hex << num;
  return ss.str();
}

}  // namespace

Cpu::Cpu(EventLogger& event_logger, Bus& bus, Registers& reg)
    : ticks_(0),
      last_time_(std::chrono::steady_clock::now()),
      event_logger_(event_logger),
      bus_(bus),
      reg_(reg) {
  Reset();
}

void Cpu::Reset() {
  reg_.pc = ReadAddressFromVectorTable(0xFFFC);
  // std::string event_name = "Reset: " + IntToHexString(reg_.pc);
  // event_logger_.LogEvent({.type = EventLogger::EventType::kInfo, .name =
  // event_name});
}

void Cpu::NMI() {
  bus_.Write(kStackStartAddress + reg_.sp--, reg_.pc >> 8);
  bus_.Write(kStackStartAddress + reg_.sp--, reg_.pc & 0xFF);
  bus_.Write(kStackStartAddress + reg_.sp--, reg_.status.to_ulong());
  reg_.status.set(Status::kIntDisable);
  reg_.pc = ReadAddressFromVectorTable(0xFFFA);
  //std::string event_name = "NMI: " + IntToHexString(reg_.pc);
  //event_logger_.LogEvent({.type = EventLogger::EventType::kInfo, .name = event_name});
}

void Cpu::Tick() {
  // Check for tick skew.
  ticks_++;
  if (ticks_ % 1790000 == 0) {
    auto now = std::chrono::steady_clock::now();
    auto time_passed = now - last_time_;
    std::chrono::nanoseconds cpu_period{1790000 / 1000000000};
    long long expected_num_ticks = time_passed / cpu_period;
    long long num_ticks = 1790000;
    if (expected_num_ticks > num_ticks) {
      std::cout << "Cpu slipped " << (expected_num_ticks - num_ticks)
                << " ticks behind" << std::endl;
    }
    last_time_ = now;
  }

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
    std::string event_name = "Failed to read from vector table address: " + IntToHexString(address);
    event_logger_.LogEvent(
        {.type = EventLogger::EventType::kError, .name = event_name});
    return 0;
  }
  return (start_address_high << 8) | start_address_low;
}
