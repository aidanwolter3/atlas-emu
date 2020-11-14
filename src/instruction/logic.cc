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
      bus_.Read(ZeroPage(), &value);
      reg_.acc &= value;
      break;
    case 0x35:
      bus_.Read(IndexedZeroPage(reg_.x), &value);
      reg_.acc &= value;
      break;
    case 0x2D:
      bus_.Read(Absolute(), &value);
      reg_.acc &= value;
      break;
    case 0x3D:
      bus_.Read(IndexedAbsolute(reg_.x), &value);
      reg_.acc &= value;
      break;
    case 0x39:
      bus_.Read(IndexedAbsolute(reg_.y), &value);
      reg_.acc &= value;
      break;
    case 0x21:
      bus_.Read(IndexedIndirect(reg_.x), &value);
      reg_.acc &= value;
      break;
    case 0x31:
      bus_.Read(IndirectIndexed(reg_.y), &value);
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
      bus_.Read(ZeroPage(), &value);
      reg_.acc |= value;
      break;
    case 0x15:
      bus_.Read(IndexedZeroPage(reg_.x), &value);
      reg_.acc |= value;
      break;
    case 0x0D:
      bus_.Read(Absolute(), &value);
      reg_.acc |= value;
      break;
    case 0x1D:
      bus_.Read(IndexedAbsolute(reg_.x), &value);
      reg_.acc |= value;
      break;
    case 0x19:
      bus_.Read(IndexedAbsolute(reg_.y), &value);
      reg_.acc |= value;
      break;
    case 0x01:
      bus_.Read(IndexedIndirect(reg_.x), &value);
      reg_.acc |= value;
      break;
    case 0x11:
      bus_.Read(IndirectIndexed(reg_.y), &value);
      reg_.acc |= value;
      break;
    default:
      std::cout << "Unsupported ORA variant: " << opcode << std::endl;
      return;
  }
  SetStatusFromData(reg_, reg_.acc);
}
