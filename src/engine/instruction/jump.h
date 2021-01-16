#ifndef ENGINE_INSTRUCTION_JUMP_H_
#define ENGINE_INSTRUCTION_JUMP_H_

#include "src/engine/instruction/instruction.h"

// Jump
class JMP : public Instruction {
 public:
  using Instruction::Instruction;
  uint8_t Execute(uint16_t operand) override;
};

// Jump Subroutine
class JSR : public Instruction {
 public:
  using Instruction::Instruction;
  uint8_t Execute(uint16_t operand) override;
};

// Return From Subroutine
class RTS : public Instruction {
 public:
  using Instruction::Instruction;
  uint8_t Execute(uint16_t operand) override;
};

// Return From Interrupt
class RTI : public Instruction {
 public:
  using Instruction::Instruction;
  uint8_t Execute(uint16_t operand) override;
};

#endif  // ENGINE_INSTRUCTION_JUMP_H_
