#ifndef ENGINE_INSTRUCTION_MATH_H_
#define ENGINE_INSTRUCTION_MATH_H_

#include "src/engine/instruction/instruction.h"

// Add with Carry
class ADC : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  uint8_t Execute(uint8_t opcode, uint16_t operand) override;
};

// Subtract with Carry
class SBC : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  uint8_t Execute(uint8_t opcode, uint16_t operand) override;
};

// Decrement Memory
class DEC : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  uint8_t Execute(uint8_t opcode, uint16_t operand) override;
};

// Decrement X
class DEX : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  uint8_t Execute(uint8_t opcode, uint16_t operand) override;
};

// Decrement Y
class DEY : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  uint8_t Execute(uint8_t opcode, uint16_t operand) override;
};

// Increment Memory
class INC : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  uint8_t Execute(uint8_t opcode, uint16_t operand) override;
};

// Increment X
class INX : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  uint8_t Execute(uint8_t opcode, uint16_t operand) override;
};

// Increment Y
class INY : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  uint8_t Execute(uint8_t opcode, uint16_t operand) override;
};

#endif  // ENGINE_INSTRUCTION_MATH_H_
