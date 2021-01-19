#include "src/engine/instruction/shift.h"

#include <cstdint>

Instruction::ExecuteResult ASL::Execute(uint16_t operand) {
  uint8_t data_before = operand & 0xFF;
  uint8_t data_after = data_before << 1;

  reg_.status.set(Status::kCarry, data_before & 0x80);
  reg_.status.set(Status::kZero, data_after == 0);
  reg_.status.set(Status::kSign, static_cast<int8_t>(data_after) < 0);

  return {.data = data_after};
}

Instruction::ExecuteResult LSR::Execute(uint16_t operand) {
  uint8_t data_before = operand & 0xFF;
  uint8_t data_after = data_before >> 1;

  reg_.status.set(Status::kCarry, data_before & 0x01);
  reg_.status.set(Status::kZero, data_after == 0);
  reg_.status.set(Status::kSign, static_cast<int8_t>(data_after) < 0);

  return {.data = data_after};
}

Instruction::ExecuteResult ROL::Execute(uint16_t operand) {
  uint8_t data_before = operand & 0xFF;
  uint8_t carry = reg_.status.test(Status::kCarry) ? 0x01 : 0x00;
  uint8_t data_after = data_before << 1 | carry;

  reg_.status.set(Status::kCarry, data_before & 0x80);
  reg_.status.set(Status::kZero, data_after == 0);
  reg_.status.set(Status::kSign, static_cast<int8_t>(data_after) < 0);

  return {.data = data_after};
}

Instruction::ExecuteResult ROR::Execute(uint16_t operand) {
  uint8_t data_before = operand & 0xFF;
  uint8_t carry = reg_.status.test(Status::kCarry) ? 0x80 : 0x00;
  uint8_t data_after = data_before >> 1 | carry;

  reg_.status.set(Status::kCarry, data_before & 0x01);
  reg_.status.set(Status::kZero, data_after == 0);
  reg_.status.set(Status::kSign, static_cast<int8_t>(data_after) < 0);

  return {.data = data_after};
}
