#include "src/instruction/load.h"

#include <cstdint>
#include <iostream>
#include <vector>

namespace {

void SetStatusFromData(Registers& reg, uint8_t data) {
  reg.status.set(Status::kZero, data == 0);
  reg.status.set(Status::kSign, static_cast<int8_t>(data) < 0);
}

}  // namespace

void LDA::ExecuteInternal(uint8_t opcode) {
  switch (opcode) {
    case 0xA9:
      reg_.acc = Immediate();
      break;
    case 0xA5:
      mem_.Read(ZeroPage(), &reg_.acc);
      break;
    case 0xB5:
      mem_.Read(IndexedZeroPage(reg_.x), &reg_.acc);
      break;
    case 0xAD:
      mem_.Read(Absolute(), &reg_.acc);
      break;
    case 0xBD:
      mem_.Read(IndexedAbsolute(reg_.x), &reg_.acc);
      break;
    case 0xB9:
      mem_.Read(IndexedAbsolute(reg_.y), &reg_.acc);
      break;
    case 0xA1:
      mem_.Read(IndexedIndirect(reg_.x), &reg_.acc);
      break;
    case 0xB1:
      mem_.Read(IndirectIndexed(reg_.y), &reg_.acc);
      break;
    default:
      std::cout << "Unsupported LDA variant: " << opcode << std::endl;
      return;
  }
  SetStatusFromData(reg_, reg_.acc);
}

void LDX::ExecuteInternal(uint8_t opcode) {
  switch (opcode) {
    case 0xA2:
      reg_.x = Immediate();
      break;
    case 0xA6:
      mem_.Read(ZeroPage(), &reg_.x);
      break;
    case 0xB6:
      mem_.Read(IndexedZeroPage(reg_.y), &reg_.x);
      break;
    case 0xAE:
      mem_.Read(Absolute(), &reg_.x);
      break;
    case 0xBE:
      mem_.Read(IndexedAbsolute(reg_.y), &reg_.x);
      break;
    default:
      std::cout << "Unsupported LDX variant: " << opcode << std::endl;
      return;
  }
  SetStatusFromData(reg_, reg_.x);
}

void LDY::ExecuteInternal(uint8_t opcode) {
  switch (opcode) {
    case 0xA0:
      reg_.y = Immediate();
      break;
    case 0xA4:
      mem_.Read(ZeroPage(), &reg_.y);
      break;
    case 0xB4:
      mem_.Read(IndexedZeroPage(reg_.x), &reg_.y);
      break;
    case 0xAC:
      mem_.Read(Absolute(), &reg_.y);
      break;
    case 0xBC:
      mem_.Read(IndexedAbsolute(reg_.x), &reg_.y);
      break;
    default:
      std::cout << "Unsupported LDY variant: " << opcode << std::endl;
      return;
  }
  SetStatusFromData(reg_, reg_.y);
}
