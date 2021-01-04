#include "src/engine/instruction/math.h"

#include <cmath>
#include <iostream>

namespace {

#define IS_POSITIVE(n) (n & 0x80 ? false : true)
#define ONES_COMPLIMENT(n) (0xFF - n)

void SetZeroSignStatus(Registers& reg, uint8_t data) {
  reg.status.set(Status::kZero, data == 0);
  reg.status.set(Status::kSign, !IS_POSITIVE(data));
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
  uint8_t b;
  uint8_t c = reg_.status.test(Status::kCarry) ? 1 : 0;
  switch (opcode) {
    case 0x69:
      b = Immediate();
      break;
    case 0x65:
      bus_.Read(ZeroPage(), &b);
      break;
    case 0x75:
      bus_.Read(IndexedZeroPage(reg_.x), &b);
      break;
    case 0x6D:
      bus_.Read(Absolute(), &b);
      break;
    case 0x7D:
      bus_.Read(IndexedAbsolute(reg_.x), &b);
      break;
    case 0x79:
      bus_.Read(IndexedAbsolute(reg_.y), &b);
      break;
    case 0x61:
      bus_.Read(IndexedIndirect(reg_.x), &b);
      break;
    case 0x71:
      bus_.Read(IndirectIndexed(reg_.y), &b);
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
  uint8_t b;
  uint8_t c = reg_.status.test(Status::kCarry) ? 1 : 0;
  switch (opcode) {
    case 0xE9:
      b = ONES_COMPLIMENT(Immediate());
      break;
    case 0xE5:
      bus_.Read(ZeroPage(), &b);
      b = ONES_COMPLIMENT(b);
      break;
    case 0xF5:
      bus_.Read(IndexedZeroPage(reg_.x), &b);
      b = ONES_COMPLIMENT(b);
      break;
    case 0xED:
      bus_.Read(Absolute(), &b);
      b = ONES_COMPLIMENT(b);
      break;
    case 0xFD:
      bus_.Read(IndexedAbsolute(reg_.x), &b);
      b = ONES_COMPLIMENT(b);
      break;
    case 0xF9:
      bus_.Read(IndexedAbsolute(reg_.y), &b);
      b = ONES_COMPLIMENT(b);
      break;
    case 0xE1:
      bus_.Read(IndexedIndirect(reg_.x), &b);
      b = ONES_COMPLIMENT(b);
      break;
    case 0xF1:
      bus_.Read(IndirectIndexed(reg_.y), &b);
      b = ONES_COMPLIMENT(b);
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
  byte -= 1;
  bus_.Write(address, byte);
  SetZeroSignStatus(reg_, byte);
}

void DEX::ExecuteInternal(uint8_t opcode) {
  reg_.x -= 1;
  SetZeroSignStatus(reg_, reg_.x);
}

void DEY::ExecuteInternal(uint8_t opcode) {
  reg_.y -= 1;
  SetZeroSignStatus(reg_, reg_.y);
}

void INC::ExecuteInternal(uint8_t opcode) {
  uint16_t address;
  switch (opcode) {
    case 0xE6:
      address = ZeroPage();
      break;
    case 0xF6:
      address = IndexedZeroPage(reg_.x);
      break;
    case 0xEE:
      address = Absolute();
      break;
    case 0xFE:
      address = IndexedAbsolute(reg_.x);
      break;
    default:
      std::cout << "Unsupported INC variant: " << opcode << std::endl;
      return;
  }

  uint8_t byte;
  bus_.Read(address, &byte);
  byte += 1;
  bus_.Write(address, byte);
  SetZeroSignStatus(reg_, byte);
}

void INX::ExecuteInternal(uint8_t opcode) {
  reg_.x += 1;
  SetZeroSignStatus(reg_, reg_.x);
}
void INY::ExecuteInternal(uint8_t opcode) {
  reg_.y += 1;
  SetZeroSignStatus(reg_, reg_.y);
}
