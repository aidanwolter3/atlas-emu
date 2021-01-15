#ifndef ENGINE_INSTRUCTION_TRANSFER_H_
#define ENGINE_INSTRUCTION_TRANSFER_H_

#include "src/engine/instruction/instruction.h"

// Transfer A to X
class TAX : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  uint8_t Execute(uint8_t opcode, uint16_t operand) override;
};

// Transfer X to A
class TXA : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  uint8_t Execute(uint8_t opcode, uint16_t operand) override;
};

// Transfer A to Y
class TAY : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  uint8_t Execute(uint8_t opcode, uint16_t operand) override;
};

// Transfer Y to A
class TYA : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  uint8_t Execute(uint8_t opcode, uint16_t operand) override;
};

// Transfer X to stack pointer
class TXS : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  uint8_t Execute(uint8_t opcode, uint16_t operand) override;
};

// Transfer stack pointer to X
class TSX : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  uint8_t Execute(uint8_t opcode, uint16_t operand) override;
};

#endif  // ENGINE_INSTRUCTION_TRANSFER_H_
