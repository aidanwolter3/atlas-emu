#ifndef ENGINE_INSTRUCTION_STORE_H_
#define ENGINE_INSTRUCTION_STORE_H_

#include "src/engine/instruction/instruction.h"

// Store Accumulator
class STA : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  uint8_t Execute(uint8_t opcode, uint16_t operand) override;
};

// Store Index X
class STX : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  uint8_t Execute(uint8_t opcode, uint16_t operand) override;
};

// Store Index Y
class STY : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  uint8_t Execute(uint8_t opcode, uint16_t operand) override;
};

#endif  // ENGINE_INSTRUCTION_STORE_H_
