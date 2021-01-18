#include "src/engine/cpu.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "src/engine/base/constants.h"
#include "src/engine/base/log.h"

Cpu::Cpu(Bus& bus, Registers& reg)
    : bus_(bus), reg_(reg), addressing_(bus_, reg_) {}

void Cpu::Reset() {
  state_ = State::kFetchOpcode;
  nmi_ = false;
  reg_.pc = ReadAddressFromVectorTable(0xFFFC);
  reg_.sp = 0xFF;
}

void Cpu::NMI() { nmi_ = true; }

void Cpu::Tick() {
  switch (state_) {
    case State::kFetchOpcode:
      // Reset state
      opcode_ = 0;
      instruction_ticks_ = 0;
      nmi_ticks_ = 0;

      instruction_ticks_++;
      FetchOpcode();
      state_ = State::kExecuteInstruction;
      break;
    case State::kExecuteInstruction:
      instruction_ticks_++;
      if (ExecuteInstruction()) {
        if (nmi_) {
          state_ = State::kNMI;
        } else {
          state_ = State::kFetchOpcode;
        }
      }
      break;
    case State::kNMI:
      nmi_ticks_++;
      if (PerformNMI()) {
        nmi_ = false;
        state_ = State::kFetchOpcode;
      }
      break;
  }
}

void Cpu::DumpRegisters() {
  LOG(INFO) << "-- CPU --";
  LOG(INFO) << "PC=" << Log::Hex(reg_.pc);
  LOG(INFO) << "SP=" << Log::Hex(reg_.sp);
  LOG(INFO) << "Status=" << reg_.status;
  LOG(INFO) << "ACC=" << Log::Hex(reg_.acc);
  LOG(INFO) << "X=" << Log::Hex(reg_.x);
  LOG(INFO) << "Y=" << Log::Hex(reg_.y);
  LOG(INFO) << "---------";

  // Dump the stack
  LOG(INFO) << "-- Stack --";
  for (uint16_t addr = kStackStartAddress + reg_.sp + 1;
       addr < kStackStartAddress + 0x100; ++addr) {
    uint8_t stack_data;
    bus_.Read(addr, &stack_data);
    LOG(INFO) << Log::Hex(addr) << ": " << Log::Hex(stack_data);
  }
  LOG(INFO) << "-----------";
}

void Cpu::RegisterInstruction(Instruction::Config config) {
  instructions_[config.opcode] = config;
}

void Cpu::FetchOpcode() {
  auto status = bus_.Read(reg_.pc, &opcode_);
  if (status != Peripheral::Status::OK) {
    LOG(ERROR) << "Failed to read the opcode at address: " << Log::Hex(reg_.pc);
    return;
  }
  reg_.pc++;

  if (!instructions_.count(opcode_)) {
    LOG(ERROR) << "Failed to decode: unkown instruction: " << Log::Hex(opcode_);
    return;
  }
}

bool Cpu::ExecuteInstruction() {
  Instruction::Config& config = instructions_[opcode_];
  return addressing_.Execute(config, instruction_ticks_);
}

bool Cpu::PerformNMI() {
  if (nmi_ticks_ < 7) return false;
  bus_.Write(kStackStartAddress + reg_.sp--, reg_.pc >> 8);
  bus_.Write(kStackStartAddress + reg_.sp--, reg_.pc & 0xFF);
  bus_.Write(kStackStartAddress + reg_.sp--, reg_.status.to_ulong());
  reg_.status.set(Status::kIntDisable);
  reg_.pc = ReadAddressFromVectorTable(0xFFFA);
  return true;
}

uint16_t Cpu::ReadAddressFromVectorTable(uint16_t address) {
  uint8_t start_address_low, start_address_high;
  Peripheral::Status status_low, status_high;
  status_low = bus_.Read(address, &start_address_low);
  status_high = bus_.Read(address + 1, &start_address_high);
  if (status_low != Peripheral::Status::OK ||
      status_high != Peripheral::Status::OK) {
    LOG(ERROR) << "Failed to read from vector table address: "
               << Log::Hex(address);
    return 0;
  }
  return (start_address_high << 8) | start_address_low;
}
