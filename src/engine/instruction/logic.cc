#include "src/engine/instruction/logic.h"

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
      value = Immediate();
      break;
    case 0x25:
      bus_.Read(ZeroPage(), &value);
      break;
    case 0x35:
      bus_.Read(IndexedZeroPage(reg_.x), &value);
      break;
    case 0x2D:
      bus_.Read(Absolute(), &value);
      break;
    case 0x3D:
      bus_.Read(IndexedAbsolute(reg_.x), &value);
      break;
    case 0x39:
      bus_.Read(IndexedAbsolute(reg_.y), &value);
      break;
    case 0x21:
      bus_.Read(IndexedIndirect(reg_.x), &value);
      break;
    case 0x31:
      bus_.Read(IndirectIndexed(reg_.y), &value);
      break;
    default:
      std::cout << "Unsupported AND variant: " << opcode << std::endl;
      return;
  }
  reg_.acc &= value;
  SetStatusFromData(reg_, reg_.acc);
}

void ORA::ExecuteInternal(uint8_t opcode) {
  uint8_t value;
  switch (opcode) {
    case 0x09:
      value = Immediate();
      break;
    case 0x05:
      bus_.Read(ZeroPage(), &value);
      break;
    case 0x15:
      bus_.Read(IndexedZeroPage(reg_.x), &value);
      break;
    case 0x0D:
      bus_.Read(Absolute(), &value);
      break;
    case 0x1D:
      bus_.Read(IndexedAbsolute(reg_.x), &value);
      break;
    case 0x19:
      bus_.Read(IndexedAbsolute(reg_.y), &value);
      break;
    case 0x01:
      bus_.Read(IndexedIndirect(reg_.x), &value);
      break;
    case 0x11:
      bus_.Read(IndirectIndexed(reg_.y), &value);
      break;
    default:
      std::cout << "Unsupported ORA variant: " << opcode << std::endl;
      return;
  }
  reg_.acc |= value;
  SetStatusFromData(reg_, reg_.acc);
}

void EOR::ExecuteInternal(uint8_t opcode) {
  uint8_t value;
  switch (opcode) {
    case 0x49:
      value = Immediate();
      break;
    case 0x45:
      bus_.Read(ZeroPage(), &value);
      break;
    case 0x55:
      bus_.Read(IndexedZeroPage(reg_.x), &value);
      break;
    case 0x4D:
      bus_.Read(Absolute(), &value);
      break;
    case 0x5D:
      bus_.Read(IndexedAbsolute(reg_.x), &value);
      break;
    case 0x59:
      bus_.Read(IndexedAbsolute(reg_.y), &value);
      break;
    case 0x41:
      bus_.Read(IndexedIndirect(reg_.x), &value);
      break;
    case 0x51:
      bus_.Read(IndirectIndexed(reg_.y), &value);
      break;
    default:
      std::cout << "Unsupported EOR variant: " << opcode << std::endl;
      return;
  }
  reg_.acc ^= value;
  SetStatusFromData(reg_, reg_.acc);
}

void BIT::ExecuteInternal(uint8_t opcode) {
  uint8_t value;
  switch (opcode) {
    case 0x24:
      bus_.Read(ZeroPage(), &value);
      break;
    case 0x2C:
      bus_.Read(Absolute(), &value);
      break;
    default:
      std::cout << "Unsupported BIT variant: " << opcode << std::endl;
      return;
  }
  reg_.status.set(Status::kZero, (value & reg_.acc) == 0);
  reg_.status.set(Status::kSign, value & 0x80);
  reg_.status.set(Status::kOverflow, value & 0x40);
}
