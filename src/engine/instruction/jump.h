#ifndef ENGINE_INSTRUCTION_JUMP_H_
#define ENGINE_INSTRUCTION_JUMP_H_

#include "src/engine/instruction/instruction.h"

// Jump
class JMP : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  bool Execute(uint8_t opcode, Addressing::Mode mode, uint16_t operand,
               int cycle) override;
};

// Jump Subroutine
class JSR : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  bool Execute(uint8_t opcode, Addressing::Mode mode, uint16_t operand,
               int cycle) override;
};

// Return From Subroutine
class RTS : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  bool Execute(uint8_t opcode, Addressing::Mode mode, uint16_t operand,
               int cycle) override;
};

// Return From Interrupt
class RTI : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  bool Execute(uint8_t opcode, Addressing::Mode mode, uint16_t operand,
               int cycle) override;
};

#endif  // ENGINE_INSTRUCTION_JUMP_H_
