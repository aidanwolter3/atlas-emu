#ifndef INSTRUCTION_LOAD_H_
#define INSTRUCTION_LOAD_H_

#include "src/public/instruction.h"

// Load Accumulator
class LDA : public Instruction {
 public:
  using Instruction::Instruction;
  void Execute(uint8_t opcode) override;
};

// Load Index X
class LDX : public Instruction {
 public:
  using Instruction::Instruction;
  void Execute(uint8_t opcode) override;
};

// Load Index Y
class LDY : public Instruction {
 public:
  using Instruction::Instruction;
  void Execute(uint8_t opcode) override;
};

#endif  // INSTRUCTION_LOAD_H_
