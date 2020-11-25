#include "src/instruction/math.h"

#include <cmath>
#include <iostream>

namespace {

#define IS_POSITIVE(n) (n & 0x80 ? true : false)
#define ONES_COMPLIMENT(n) (0xFF - n)

void SetZeroSignStatus(Registers& reg, uint8_t data) {
  reg.status.set(Status::kZero, data == 0);
  reg.status.set(Status::kSign, static_cast<int8_t>(data) < 0);
}

void SetStatusFromSummation(Registers& reg, uint16_t a, uint16_t b,
                            uint16_t sum) {
  SetZeroSignStatus(reg, sum);
  reg.status.set(Status::kCarry, sum & 0x0100);

  // Set the overflow flag if two positives made a negative, or two negatives
  // made a positive.
  if ((IS_POSITIVE(a) && IS_POSITIVE(b) && !IS_POSITIVE(sum)) ||
      (!IS_POSITIVE(a) && !IS_POSITIVE(b) && IS_POSITIVE(sum))) {
    reg.status.set(Status::kOverflow);
  }
}

}  // namespace

void ADC::ExecuteInternal(uint8_t opcode) {
  uint16_t a = reg_.acc;
  uint16_t b;
  uint16_t c = reg_.status.test(Status::kCarry) ? 1 : 0;
  switch (opcode) {
    case 0x69:
      b = Immediate();
      break;
    case 0x65:
      b = ZeroPage();
      break;
    case 0x75:
      b = IndexedZeroPage(reg_.x);
      break;
    case 0x6D:
      b = Absolute();
      break;
    case 0x7D:
      b = IndexedAbsolute(reg_.x);
      break;
    case 0x79:
      b = IndexedAbsolute(reg_.y);
      break;
    case 0x61:
      b = IndexedIndirect(reg_.x);
      break;
    case 0x71:
      b = IndirectIndexed(reg_.y);
      break;
    default:
      std::cout << "Unsupported ADC variant: " << opcode << std::endl;
      return;
  }

  uint16_t sum = a + b + c;
  SetStatusFromSummation(reg_, a, b, sum);
  reg_.acc = sum;
}

void SBC::ExecuteInternal(uint8_t opcode) {
  uint16_t a = reg_.acc;
  uint16_t b;
  uint16_t c = reg_.status.test(Status::kCarry) ? 1 : 0;
  switch (opcode) {
    case 0xE9:
      b = ONES_COMPLIMENT(Immediate());
      break;
    case 0xE5:
      b = ONES_COMPLIMENT(ZeroPage());
      break;
    case 0xF5:
      b = ONES_COMPLIMENT(IndexedZeroPage(reg_.x));
      break;
    case 0xED:
      b = ONES_COMPLIMENT(IndexedZeroPage(reg_.y));
      break;
    case 0xFD:
      b = ONES_COMPLIMENT(Absolute());
      break;
    case 0xF9:
      b = ONES_COMPLIMENT(IndexedAbsolute(reg_.x));
      break;
    case 0xE1:
      b = ONES_COMPLIMENT(IndexedIndirect(reg_.x));
      break;
    case 0xF1:
      b = ONES_COMPLIMENT(IndirectIndexed(reg_.y));
      break;
    default:
      std::cout << "Unsupported SBC variant: " << opcode << std::endl;
      return;
  }

  uint16_t sum = a + b + c;
  SetStatusFromSummation(reg_, a, b, sum);
  reg_.acc = sum;
}

void DEC::ExecuteInternal(uint8_t opcode) {
  uint16_t address;
  switch (opcode) {
    case 0xC6:
      address = ZeroPage();
      break;
    case 0xD6:
      address = IndexedZeroPage(reg_.x);
      break;
    case 0xCE:
      address = Absolute();
      break;
    case 0xDE:
      address = IndexedAbsolute(reg_.x);
      break;
    default:
      std::cout << "Unsupported DEC variant: " << opcode << std::endl;
      return;
  }

  uint8_t byte;
  bus_.Read(address, &byte);
  bus_.Write(address, byte + 1);
  SetZeroSignStatus(reg_, byte);
}
