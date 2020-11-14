#include "src/instruction/logic.h"

#include <iostream>

namespace {

void SetStatusFromData(Registers& reg, uint8_t data) {
  reg.status.set(Status::kZero, data == 0);
  reg.status.set(Status::kSign, static_cast<int8_t>(data) < 0);
}

}  // namespace

void AND::ExecuteInternal(uint8_t opcode) {
  uint8_t value;
  switch (opcode) {
    case 0x29:
      reg_.acc &= Immediate();
      break;
    case 0x25:
      mem_.Read(ZeroPage(), &value);
      reg_.acc &= value;
      break;
    case 0x35:
      mem_.Read(IndexedZeroPage(reg_.x), &value);
      reg_.acc &= value;
      break;
    case 0x2D:
      mem_.Read(Absolute(), &value);
      reg_.acc &= value;
      break;
    case 0x3D:
      mem_.Read(IndexedAbsolute(reg_.x), &value);
      reg_.acc &= value;
      break;
    case 0x39:
      mem_.Read(IndexedAbsolute(reg_.y), &value);
      reg_.acc &= value;
      break;
    case 0x21:
      mem_.Read(IndexedIndirect(reg_.x), &value);
      reg_.acc &= value;
      break;
    case 0x31:
      mem_.Read(IndirectIndexed(reg_.y), &value);
      reg_.acc &= value;
      break;
    default:
      std::cout << "Unsupported AND variant: " << opcode << std::endl;
      return;
  }
  SetStatusFromData(reg_, reg_.acc);
}

void ORA::ExecuteInternal(uint8_t opcode) {
  uint8_t value;
  switch (opcode) {
    case 0x09:
      reg_.acc |= Immediate();
      break;
    case 0x05:
      mem_.Read(ZeroPage(), &value);
      reg_.acc |= value;
      break;
    case 0x15:
      mem_.Read(IndexedZeroPage(reg_.x), &value);
      reg_.acc |= value;
      break;
    case 0x0D:
      mem_.Read(Absolute(), &value);
      reg_.acc |= value;
      break;
    case 0x1D:
      mem_.Read(IndexedAbsolute(reg_.x), &value);
      reg_.acc |= value;
      break;
    case 0x19:
      mem_.Read(IndexedAbsolute(reg_.y), &value);
      reg_.acc |= value;
      break;
    case 0x01:
      mem_.Read(IndexedIndirect(reg_.x), &value);
      reg_.acc |= value;
      break;
    case 0x11:
      mem_.Read(IndirectIndexed(reg_.y), &value);
      reg_.acc |= value;
      break;
    default:
      std::cout << "Unsupported ORA variant: " << opcode << std::endl;
      return;
  }
  SetStatusFromData(reg_, reg_.acc);
}
