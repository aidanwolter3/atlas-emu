#ifndef INSTRUCTION_LOAD_H_
#define INSTRUCTION_LOAD_H_

#include "src/public/instruction.h"

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

#endif  // INSTRUCTION_LOAD_H_
