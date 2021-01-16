#ifndef ENGINE_INSTRUCTION_LOGIC_H_
#define ENGINE_INSTRUCTION_LOGIC_H_

#include "src/engine/instruction/instruction.h"

// AND Accumulator
class AND : public Instruction {
 public:
  using Instruction::Instruction;
  uint8_t Execute(uint16_t operand) override;
};

// OR Accumulator
class ORA : public Instruction {
 public:
  using Instruction::Instruction;
  uint8_t Execute(uint16_t operand) override;
};

// Exclusive OR Accumulator
class EOR : public Instruction {
 public:
  using Instruction::Instruction;
  uint8_t Execute(uint16_t operand) override;
};

// Bit Test
class BIT : public Instruction {
 public:
  using Instruction::Instruction;
  uint8_t Execute(uint16_t operand) override;
};

#endif  // ENGINE_INSTRUCTION_LOGIC_H_
