#include "src/engine/public/instruction.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace {

constexpr bool kDebug = false;

std::string IntToHexString(int num) {
  std::stringstream ss;
  ss << std::setfill('0') << std::setw(2) << std::hex << num;
  return ss.str();
}

}  // namespace

void Instruction::Execute(uint8_t opcode) {
  if (kDebug) {
    // The PC has already been incremented by the CPU, so decrement one to find
    // the instruction.
    uint16_t pc_of_instruction = reg_.pc - 1;
    log_elements_.clear();
    log_elements_.push_back("(0x" + IntToHexString(pc_of_instruction) + ")");
    log_elements_.push_back(GetLogName());
  }

  ExecuteInternal(opcode);

  if (kDebug) {
    for (auto el : log_elements_) {
      std::cout << el << " ";
    }
    std::cout << std::endl;
  }
}

std::string Instruction::GetLogName() { return "<unknown>"; }

std::vector<uint8_t> Instruction::FetchOperands(int num) {
  std::vector<uint8_t> operands(num);
  uint16_t pc = reg_.pc;
  for (int i = 0; i < num; ++i) {
    auto status = bus_.Read(pc++, &operands[i]);
    if (status != Peripheral::Status::OK) {
      std::cout << "Failed to fetch operands" << std::endl;
      return {};
    }
  }
  reg_.pc = pc;
  return operands;
}

uint16_t Instruction::Immediate() {
  std::vector<uint8_t> operands = FetchOperands(1);
  uint16_t data = operands[0];
  if (kDebug) {
    log_elements_.push_back("#" + IntToHexString(data));
  }
  return data;
}

uint16_t Instruction::ZeroPage() {
  std::vector<uint8_t> operands = FetchOperands(1);
  uint16_t address = operands[0];
  if (kDebug) {
    log_elements_.push_back("$" + IntToHexString(address));
  }
  return address;
}

uint16_t Instruction::IndexedZeroPage(uint8_t index) {
  uint16_t address = ZeroPage() + index;
  if (kDebug) {
    log_elements_.push_back("+ " + IntToHexString(index));
  }
  return address;
}

uint16_t Instruction::Absolute() {
  std::vector<uint8_t> operands = FetchOperands(2);
  uint16_t address = ((operands[1] << 8) | operands[0]);
  if (kDebug) {
    log_elements_.push_back("$" + IntToHexString(address));
  }
  return address;
}

// IndirectAbsolute() is only used by JMP.
// TODO: Move to that instruction.
uint16_t Instruction::IndirectAbsolute() {
  std::vector<uint8_t> operands = FetchOperands(2);
  uint16_t address_location_1 = ((operands[1] << 8) | operands[0]);

  // The lower byte of the address does not carry when incremented.
  // 0xABFF + 1 = 0xAB00
  uint16_t address_location_2 = address_location_1;
  address_location_2 &= 0xFF00;
  address_location_2 |= static_cast<int8_t>(address_location_1 & 0xFF) + 1;

  uint8_t lower_byte, upper_byte;
  bus_.Read(address_location_1, &lower_byte);
  bus_.Read(address_location_2, &upper_byte);
  uint16_t address = (upper_byte << 8 | lower_byte);

  if (kDebug) {
    log_elements_.push_back("($" + IntToHexString(address_location_1) +
                           ") = " + IntToHexString(address));
   }
  return address;
}

uint16_t Instruction::IndexedAbsolute(uint8_t index) {
  uint16_t address = Absolute() + index;
  if (kDebug) {
    log_elements_.push_back("+ " + IntToHexString(index));
  }
  return address;
}

uint16_t Instruction::IndexedIndirect(uint8_t index) {
  std::vector<uint8_t> operands = FetchOperands(1);

  uint8_t address_low, address_high;
  bus_.Read(operands[0] + index, &address_low);
  bus_.Read(operands[0] + index + 1, &address_high);
  uint16_t address = (address_high << 8) | address_low;

  if (kDebug) {
    log_elements_.push_back("($" + IntToHexString(operands[0]) + " + " +
                           IntToHexString(index) + ") = $" +
                           IntToHexString(address));
  }
  return address;
}

uint16_t Instruction::IndirectIndexed(uint8_t index) {
  std::vector<uint8_t> operands = FetchOperands(1);

  uint8_t address_low, address_high;
  bus_.Read(operands[0], &address_low);
  bus_.Read(operands[0] + 1, &address_high);
  uint16_t address = (address_high << 8) | address_low;
  address += index;

  if (kDebug) {
    log_elements_.push_back("($" + IntToHexString(operands[0]) + ") + " +
                           IntToHexString(index) + " = $" +
                           IntToHexString(address));
  }
  return address;
}
