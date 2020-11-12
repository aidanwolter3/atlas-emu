#ifndef INSTRUCTION_STATUS_H_
#define INSTRUCTION_STATUS_H_

#include "src/public/instruction.h"

// Clear Carry
class CLC : public Instruction {
 public:
  using Instruction::Instruction;
  void Execute(uint8_t opcode) override;
};

// Set Carry
class SEC : public Instruction {
 public:
  using Instruction::Instruction;
  void Execute(uint8_t opcode) override;
};

// Clear Interrupt
class CLI : public Instruction {
 public:
  using Instruction::Instruction;
  void Execute(uint8_t opcode) override;
};

// Set Interrupt
class SEI : public Instruction {
 public:
  using Instruction::Instruction;
  void Execute(uint8_t opcode) override;
};

// Clear Overflow
class CLV : public Instruction {
 public:
  using Instruction::Instruction;
  void Execute(uint8_t opcode) override;
};

// Clear Decimal
class CLD : public Instruction {
 public:
  using Instruction::Instruction;
  void Execute(uint8_t opcode) override;
};

// Set Decimal
class SED : public Instruction {
 public:
  using Instruction::Instruction;
  void Execute(uint8_t opcode) override;
};

#endif  // INSTRUCTION_STATUS_H_
