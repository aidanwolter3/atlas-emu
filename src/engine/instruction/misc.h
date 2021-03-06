#ifndef ENGINE_INSTRUCTION_MISC_H_
#define ENGINE_INSTRUCTION_MISC_H_

#include "src/engine/instruction/instruction.h"

// No Operation
class NOP : public Instruction {
 public:
  using Instruction::Instruction;
  ExecuteResult Execute(uint16_t operand) override;
  SET_LOG_NAME("NOP");
};

// Break (used for e2e tests)
class BRK : public Instruction {
 public:
  using Instruction::Instruction;
  ExecuteResult Execute(uint16_t operand) override;
  SET_LOG_NAME("BRK");
};

#endif  // ENGINE_INSTRUCTION_MISC_H_
