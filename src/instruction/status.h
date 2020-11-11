#ifndef INSTRUCTION_STATUS_H_
#define INSTRUCTION_STATUS_H_

#include "src/public/instruction.h"

// Set Interrupt Disable Status
class SEI : public Instruction {
 public:
  using Instruction::Instruction;
  void Execute(uint8_t opcode) override;
};

// Clear Decimal Mode
class CLD : public Instruction {
 public:
  using Instruction::Instruction;
  void Execute(uint8_t opcode) override;
};

#endif  // INSTRUCTION_STATUS_H_
