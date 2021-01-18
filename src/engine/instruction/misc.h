#ifndef ENGINE_INSTRUCTION_MISC_H_
#define ENGINE_INSTRUCTION_MISC_H_

#include "src/engine/instruction/instruction.h"

// No Operation
class NOP : public Instruction {
 public:
  using Instruction::Instruction;
  uint8_t Execute(uint16_t operand) override;
};

// Break (used for e2e tests)
class BRK : public Instruction {
 public:
  using Instruction::Instruction;
  uint8_t Execute(uint16_t operand) override;
};

#endif  // ENGINE_INSTRUCTION_MISC_H_
