#include "src/engine/instruction/math.h"

namespace {

#define IS_POSITIVE(n) (n & 0x80 ? false : true)
#define ONES_COMPLIMENT(n) (0xFF - (n))

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

uint8_t ADC::Execute(uint16_t operand) {
  uint16_t a = reg_.acc;
  uint8_t b = operand & 0xFF;
  uint8_t c = reg_.status.test(Status::kCarry) ? 1 : 0;
  uint16_t sum = a + b + c;
  SetStatusFromSummation(reg_, a, b, sum);
  reg_.acc = sum;
  return 0;
}

uint8_t SBC::Execute(uint16_t operand) {
  uint16_t a = reg_.acc;
  uint8_t b = ONES_COMPLIMENT(operand & 0xFF);
  uint8_t c = reg_.status.test(Status::kCarry) ? 1 : 0;
  uint16_t sum = a + b + c;
  SetStatusFromSummation(reg_, a, b, sum);
  reg_.acc = sum;
  return 0;
}

uint8_t DEC::Execute(uint16_t operand) {
  uint8_t result = (operand & 0xFF) - 1;
  SetZeroSignStatus(reg_, result);
  return result;
}

uint8_t DEX::Execute(uint16_t operand) {
  reg_.x -= 1;
  SetZeroSignStatus(reg_, reg_.x);
  return 0;
}

uint8_t DEY::Execute(uint16_t operand) {
  reg_.y -= 1;
  SetZeroSignStatus(reg_, reg_.y);
  return 0;
}

uint8_t INC::Execute(uint16_t operand) {
  uint8_t result = (operand & 0xFF) + 1;
  SetZeroSignStatus(reg_, result);
  return result;
}

uint8_t INX::Execute(uint16_t operand) {
  reg_.x += 1;
  SetZeroSignStatus(reg_, reg_.x);
  return 0;
}

uint8_t INY::Execute(uint16_t operand) {
  reg_.y += 1;
  SetZeroSignStatus(reg_, reg_.y);
  return 0;
}
