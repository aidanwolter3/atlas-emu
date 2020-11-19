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
      bus_.Read(ZeroPage(), &reg_.acc);
      break;
    case 0xB5:
      bus_.Read(IndexedZeroPage(reg_.x), &reg_.acc);
      break;
    case 0xAD:
      bus_.Read(Absolute(), &reg_.acc);
      break;
    case 0xBD:
      bus_.Read(IndexedAbsolute(reg_.x), &reg_.acc);
      break;
    case 0xB9:
      bus_.Read(IndexedAbsolute(reg_.y), &reg_.acc);
      break;
    case 0xA1:
      bus_.Read(IndexedIndirect(reg_.x), &reg_.acc);
      break;
    case 0xB1:
      bus_.Read(IndirectIndexed(reg_.y), &reg_.acc);
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
      bus_.Read(ZeroPage(), &reg_.x);
      break;
    case 0xB6:
      bus_.Read(IndexedZeroPage(reg_.y), &reg_.x);
      break;
    case 0xAE:
      bus_.Read(Absolute(), &reg_.x);
      break;
    case 0xBE:
      bus_.Read(IndexedAbsolute(reg_.y), &reg_.x);
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
      bus_.Read(ZeroPage(), &reg_.y);
      break;
    case 0xB4:
      bus_.Read(IndexedZeroPage(reg_.x), &reg_.y);
      break;
    case 0xAC:
      bus_.Read(Absolute(), &reg_.y);
      break;
    case 0xBC:
      bus_.Read(IndexedAbsolute(reg_.x), &reg_.y);
      break;
    default:
      std::cout << "Unsupported LDY variant: " << opcode << std::endl;
      return;
  }
  SetStatusFromData(reg_, reg_.y);
}
