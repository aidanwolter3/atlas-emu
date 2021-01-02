#ifndef ENGINE_INSTRUCTION_LOAD_H_
#define ENGINE_INSTRUCTION_LOAD_H_

#include "src/engine/public/instruction.h"

// Load Accumulator
class LDA : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("LDA");
};

// Load Index X
class LDX : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("LDX");
};

// Load Index Y
class LDY : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("LDY");
};

#endif  // ENGINE_INSTRUCTION_LOAD_H_