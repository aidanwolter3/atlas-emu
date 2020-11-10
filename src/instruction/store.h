#ifndef INSTRUCTION_STORE_H_
#define INSTRUCTION_STORE_H_

#include "src/instruction/instruction.h"

// Store Accumulator
class STA : public Instruction {
 public:
  using Instruction::Instruction;
  void Execute(uint8_t opcode) override;
};

#endif  // INSTRUCTION_STORE_H_
