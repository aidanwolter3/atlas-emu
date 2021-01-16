#ifndef ENGINE_INSTRUCTION_LOAD_H_
#define ENGINE_INSTRUCTION_LOAD_H_

#include "src/engine/instruction/instruction.h"

// Load Accumulator
class LDA : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  uint8_t Execute(uint8_t opcode, uint16_t operand) override;
};

// Load Index X
class LDX : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  uint8_t Execute(uint8_t opcode, uint16_t operand) override;
};

// Load Index Y
class LDY : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  uint8_t Execute(uint8_t opcode, uint16_t operand) override;
};

#endif  // ENGINE_INSTRUCTION_LOAD_H_
