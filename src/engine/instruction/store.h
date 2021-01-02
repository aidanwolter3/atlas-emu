#ifndef ENGINE_INSTRUCTION_STORE_H_
#define ENGINE_INSTRUCTION_STORE_H_

#include "src/public/instruction.h"

// Store Accumulator
class STA : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("STA");
};

// Store Index X
class STX : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("STX");
};

// Store Index Y
class STY : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("STY");
};

#endif  // ENGINE_INSTRUCTION_STORE_H_
