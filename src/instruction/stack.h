#ifndef INSTRUCTION_STACK_H_
#define INSTRUCTION_STACK_H_

#include "src/public/instruction.h"

// Transfer X to stack pointer
class TXS : public Instruction {
 public:
  using Instruction::Instruction;
  void Execute(uint8_t opcode) override;
};

// Transfer stack pointer to X
class TSX : public Instruction {
 public:
  using Instruction::Instruction;
  void Execute(uint8_t opcode) override;
};

// Push Accumulator
class PHA : public Instruction {
 public:
  using Instruction::Instruction;
  void Execute(uint8_t opcode) override;
};

// Pull Accumulator
class PLA : public Instruction {
 public:
  using Instruction::Instruction;
  void Execute(uint8_t opcode) override;
};

// Push Status
class PHP : public Instruction {
 public:
  using Instruction::Instruction;
  void Execute(uint8_t opcode) override;
};

// Pull Status
class PLP : public Instruction {
 public:
  using Instruction::Instruction;
  void Execute(uint8_t opcode) override;
};

#endif  // INSTRUCTION_STACK_H_
