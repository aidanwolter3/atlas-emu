#ifndef ENGINE_INSTRUCTION_COMPARE_H_
#define ENGINE_INSTRUCTION_COMPARE_H_

#include "src/engine/instruction/instruction.h"

// Compare Accumulator
class CMP : public Instruction {
 public:
  using Instruction::Instruction;
  uint8_t Execute(uint16_t operand) override;
};

// Compare X Register
class CPX : public Instruction {
 public:
  using Instruction::Instruction;
  uint8_t Execute(uint16_t operand) override;
};

// Compare Y Register
class CPY : public Instruction {
 public:
  using Instruction::Instruction;
  uint8_t Execute(uint16_t operand) override;
};

#endif  // ENGINE_INSTRUCTION_COMPARE_H_
