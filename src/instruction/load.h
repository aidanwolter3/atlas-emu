#ifndef INSTRUCTION_LOAD_H_
#define INSTRUCTION_LOAD_H_

#include "src/instruction/instruction.h"

// Load Accumulator
class LDA : public Instruction {
 public:
  using Instruction::Instruction;
  void Execute(uint8_t opcode) override;
};

#endif  // INSTRUCTION_LOAD_H_
