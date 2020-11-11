#ifndef INSTRUCTION_MISC_H_
#define INSTRUCTION_MISC_H_

#include "src/public/instruction.h"

// No Operation
class NOP : public Instruction {
 public:
  using Instruction::Instruction;
  void Execute(uint8_t opcode) override;
};

#endif  // INSTRUCTION_MISC_H_
