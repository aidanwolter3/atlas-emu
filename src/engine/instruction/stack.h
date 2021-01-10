#ifndef ENGINE_INSTRUCTION_STACK_H_
#define ENGINE_INSTRUCTION_STACK_H_

#include "src/engine/instruction/instruction.h"

// Push Accumulator
class PHA : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  bool Execute(uint8_t opcode, Addressing::Mode mode, uint16_t operand,
               int cycle) override;
};

// Pull Accumulator
class PLA : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  bool Execute(uint8_t opcode, Addressing::Mode mode, uint16_t operand,
               int cycle) override;
};

// Push Status
class PHP : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  bool Execute(uint8_t opcode, Addressing::Mode mode, uint16_t operand,
               int cycle) override;
};

// Pull Status
class PLP : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  bool Execute(uint8_t opcode, Addressing::Mode mode, uint16_t operand,
               int cycle) override;
};

#endif  // ENGINE_INSTRUCTION_STACK_H_
