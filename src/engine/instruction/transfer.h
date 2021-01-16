#ifndef ENGINE_INSTRUCTION_TRANSFER_H_
#define ENGINE_INSTRUCTION_TRANSFER_H_

#include "src/engine/instruction/instruction.h"

// Transfer A to X
class TAX : public Instruction {
 public:
  using Instruction::Instruction;
  uint8_t Execute(uint16_t operand) override;
};

// Transfer X to A
class TXA : public Instruction {
 public:
  using Instruction::Instruction;
  uint8_t Execute(uint16_t operand) override;
};

// Transfer A to Y
class TAY : public Instruction {
 public:
  using Instruction::Instruction;
  uint8_t Execute(uint16_t operand) override;
};

// Transfer Y to A
class TYA : public Instruction {
 public:
  using Instruction::Instruction;
  uint8_t Execute(uint16_t operand) override;
};

// Transfer X to stack pointer
class TXS : public Instruction {
 public:
  using Instruction::Instruction;
  uint8_t Execute(uint16_t operand) override;
};

// Transfer stack pointer to X
class TSX : public Instruction {
 public:
  using Instruction::Instruction;
  uint8_t Execute(uint16_t operand) override;
};

#endif  // ENGINE_INSTRUCTION_TRANSFER_H_
