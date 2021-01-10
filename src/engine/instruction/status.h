#ifndef ENGINE_INSTRUCTION_STATUS_H_
#define ENGINE_INSTRUCTION_STATUS_H_

#include "src/engine/instruction/instruction.h"

// Clear Carry
class CLC : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  bool Execute(uint8_t opcode, uint16_t operand, int cycle) override;
};

// Set Carry
class SEC : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  bool Execute(uint8_t opcode, uint16_t operand, int cycle) override;
};

// Clear Interrupt
class CLI : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  bool Execute(uint8_t opcode, uint16_t operand, int cycle) override;
};

// Set Interrupt
class SEI : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  bool Execute(uint8_t opcode, uint16_t operand, int cycle) override;
};

// Clear Overflow
class CLV : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  bool Execute(uint8_t opcode, uint16_t operand, int cycle) override;
};

// Clear Decimal
class CLD : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  bool Execute(uint8_t opcode, uint16_t operand, int cycle) override;
};

// Set Decimal
class SED : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  bool Execute(uint8_t opcode, uint16_t operand, int cycle) override;
};

#endif  // ENGINE_INSTRUCTION_STATUS_H_
