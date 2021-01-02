#ifndef ENGINE_INSTRUCTION_JUMP_H_
#define ENGINE_INSTRUCTION_JUMP_H_

#include "src/engine/public/instruction.h"

// Jump
class JMP : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("JMP");
};

// Jump Subroutine
class JSR : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("JSR");
};

// Return From Subroutine
class RTS : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("RTS");
};

// Return From Interrupt
class RTI : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("RTI");
};

#endif  // ENGINE_INSTRUCTION_JUMP_H_
