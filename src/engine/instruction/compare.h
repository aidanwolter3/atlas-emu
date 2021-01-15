#ifndef ENGINE_INSTRUCTION_COMPARE_H_
#define ENGINE_INSTRUCTION_COMPARE_H_

#include "src/engine/instruction/instruction.h"

// Compare Accumulator
class CMP : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  uint8_t Execute(uint8_t opcode, uint16_t operand) override;
};

// Compare X Register
class CPX : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  uint8_t Execute(uint8_t opcode, uint16_t operand) override;
};

// Compare Y Register
class CPY : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  uint8_t Execute(uint8_t opcode, uint16_t operand) override;
};

#endif  // ENGINE_INSTRUCTION_COMPARE_H_
