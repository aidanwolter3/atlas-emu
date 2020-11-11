#include "src/instruction/load.h"

#include <cstdint>
#include <iostream>
#include <vector>

void Load::SetStatusFromData(uint8_t data) {
  reg_.status.set(Status::kZero, data == 0);
  reg_.status.set(Status::kSign, static_cast<int8_t>(data) < 0);
}

void Load::LoadImmediate(uint8_t& dest) {
  std::vector<uint8_t> operands = FetchOperands(1);
  dest = operands[0];
  SetStatusFromData(dest);
}

void Load::LoadZeroPage(uint8_t& dest) { LoadIndexedZeroPage(dest, 0); }

void Load::LoadIndexedZeroPage(uint8_t& dest, uint8_t index) {
  std::vector<uint8_t> operands = FetchOperands(1);
  mem_.Read(operands[0] + index, &dest);
  SetStatusFromData(dest);
}

void Load::LoadAbsolute(uint8_t& dest) { LoadIndexedAbsolute(dest, 0); }

void Load::LoadIndexedAbsolute(uint8_t& dest, uint8_t index) {
  std::vector<uint8_t> operands = FetchOperands(2);
  uint16_t address = (operands[0] << 8) | operands[1];
  mem_.Read(address + index, &dest);
  SetStatusFromData(dest);
}

void Load::LoadIndexedIndirect(uint8_t& dest, uint8_t index) {
  std::vector<uint8_t> operands = FetchOperands(1);
  uint8_t address;
  mem_.Read(operands[0] + index, &address);
  mem_.Read(address, &dest);
  SetStatusFromData(dest);
}

void Load::LoadIndirectIndexed(uint8_t& dest, uint8_t index) {
  std::vector<uint8_t> operands = FetchOperands(1);
  uint8_t address;
  mem_.Read(operands[0], &address);
  mem_.Read(address + index, &dest);
  SetStatusFromData(dest);
}

void LDA::Execute(uint8_t opcode) {
  switch (opcode) {
    case 0xA9:
      LoadImmediate(reg_.acc);
      break;
    case 0xA5:
      LoadZeroPage(reg_.acc);
      break;
    case 0xB5:
      LoadIndexedZeroPage(reg_.acc, reg_.x);
      break;
    case 0xAD:
      LoadAbsolute(reg_.acc);
      break;
    case 0xBD:
      LoadIndexedAbsolute(reg_.acc, reg_.x);
      break;
    case 0xB9:
      LoadIndexedAbsolute(reg_.acc, reg_.y);
      break;
    case 0xA1:
      LoadIndexedIndirect(reg_.acc, reg_.x);
      break;
    case 0xB1:
      LoadIndirectIndexed(reg_.acc, reg_.y);
      break;
    default:
      std::cout << "Unsupported LDA variant: " << opcode << std::endl;
      return;
  }
}

void LDX::Execute(uint8_t opcode) {
  switch (opcode) {
    case 0xA2:
      LoadImmediate(reg_.x);
      break;
    case 0xA6:
      LoadZeroPage(reg_.x);
      break;
    case 0xB6:
      LoadIndexedZeroPage(reg_.x, reg_.y);
      break;
    case 0xAE:
      LoadAbsolute(reg_.x);
      break;
    case 0xBE:
      LoadIndexedAbsolute(reg_.x, reg_.y);
      break;
    default:
      std::cout << "Unsupported LDX variant: " << opcode << std::endl;
      return;
  }
}

void LDY::Execute(uint8_t opcode) {
  switch (opcode) {
    case 0xA0:
      LoadImmediate(reg_.y);
      break;
    case 0xA4:
      LoadZeroPage(reg_.y);
      break;
    case 0xB4:
      LoadIndexedZeroPage(reg_.y, reg_.x);
      break;
    case 0xAC:
      LoadAbsolute(reg_.y);
      break;
    case 0xBC:
      LoadIndexedAbsolute(reg_.y, reg_.x);
      break;
    default:
      std::cout << "Unsupported LDY variant: " << opcode << std::endl;
      return;
  }
}
