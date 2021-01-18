#ifndef ENGINE_INSTRUCTION_STACK_H_
#define ENGINE_INSTRUCTION_STACK_H_

#include "src/engine/instruction/instruction.h"

// Push Accumulator
class PHA : public Instruction {
 public:
  using Instruction::Instruction;
  uint8_t Execute(uint16_t operand) override;
  SET_LOG_NAME("PHA");
};

// Pull Accumulator
class PLA : public Instruction {
 public:
  using Instruction::Instruction;
  uint8_t Execute(uint16_t operand) override;
  SET_LOG_NAME("PLA");
};

// Push Status
class PHP : public Instruction {
 public:
  using Instruction::Instruction;
  uint8_t Execute(uint16_t operand) override;
  SET_LOG_NAME("PHP");
};

// Pull Status
class PLP : public Instruction {
 public:
  using Instruction::Instruction;
  uint8_t Execute(uint16_t operand) override;
  SET_LOG_NAME("PLP");
};

#endif  // ENGINE_INSTRUCTION_STACK_H_
