#include "src/engine/instruction/compare.h"

#include <cstdint>
#include <iostream>

namespace {

void Compare(Registers& reg, uint8_t value, uint8_t comparison) {
  reg.status.set(Status::kZero, value == comparison);
  reg.status.set(Status::kSign, comparison > value);
  reg.status.set(Status::kCarry, value >= comparison);
}

}  // namespace

void CMP::ExecuteInternal(uint8_t opcode) {
  uint8_t comparison;
  switch (opcode) {
    case 0xC9:
      comparison = Immediate();
      break;
    case 0xC5:
      bus_.Read(ZeroPage(), &comparison);
      break;
    case 0xD5:
      bus_.Read(IndexedZeroPage(reg_.x), &comparison);
      break;
    case 0xCD:
      bus_.Read(Absolute(), &comparison);
      break;
    case 0xDD:
      bus_.Read(IndexedAbsolute(reg_.x), &comparison);
      break;
    case 0xD9:
      bus_.Read(IndexedAbsolute(reg_.y), &comparison);
      break;
    case 0xC1:
      bus_.Read(IndexedIndirect(reg_.x), &comparison);
      break;
    case 0xD1:
      bus_.Read(IndirectIndexed(reg_.y), &comparison);
      break;
    default:
      std::cout << "Unsupported CMP variant: " << opcode << std::endl;
      return;
  }
  Compare(reg_, reg_.acc, comparison);
}

void CPX::ExecuteInternal(uint8_t opcode) {
  uint8_t comparison;
  switch (opcode) {
    case 0xE0:
      comparison = Immediate();
      break;
    case 0xE4:
      bus_.Read(ZeroPage(), &comparison);
      break;
    case 0xEC:
      bus_.Read(Absolute(), &comparison);
      break;
    default:
      std::cout << "Unsupported CPX variant: " << opcode << std::endl;
      return;
  }
  Compare(reg_, reg_.x, comparison);
}

void CPY::ExecuteInternal(uint8_t opcode) {
  uint8_t comparison;
  switch (opcode) {
    case 0xC0:
      comparison = Immediate();
      break;
    case 0xC4:
      bus_.Read(ZeroPage(), &comparison);
      break;
    case 0xCC:
      bus_.Read(Absolute(), &comparison);
      break;
    default:
      std::cout << "Unsupported CPy variant: " << opcode << std::endl;
      return;
  }
  Compare(reg_, reg_.y, comparison);
}
