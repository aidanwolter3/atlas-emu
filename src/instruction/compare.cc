#include "src/instruction/compare.h"

#include <cstdint>
#include <iostream>

namespace {

void Compare(Registers& reg, uint8_t value, uint8_t comparison) {
  reg.status.set(Status::kZero, value == comparison);
  reg.status.set(Status::kSign, static_cast<int8_t>(value) < 0);
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
      comparison = ZeroPage();
      break;
    case 0xD5:
      comparison = IndexedZeroPage(reg_.x);
      break;
    case 0xCD:
      comparison = Absolute();
      break;
    case 0xDD:
      comparison = IndexedAbsolute(reg_.x);
      break;
    case 0xD9:
      comparison = IndexedAbsolute(reg_.y);
      break;
    case 0xC1:
      comparison = IndexedIndirect(reg_.x);
      break;
    case 0xD1:
      comparison = IndirectIndexed(reg_.y);
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
      comparison = ZeroPage();
      break;
    case 0xEC:
      comparison = Absolute();
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
      comparison = ZeroPage();
      break;
    case 0xCC:
      comparison = Absolute();
      break;
    default:
      std::cout << "Unsupported CPy variant: " << opcode << std::endl;
      return;
  }
  Compare(reg_, reg_.y, comparison);
}
