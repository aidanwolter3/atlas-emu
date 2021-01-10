#ifndef ENGINE_INSTRUCTION_COMPARE_H_
#define ENGINE_INSTRUCTION_COMPARE_H_

#include "src/engine/instruction/instruction.h"

// Compare Accumulator
class CMP : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("CMP");
};

// Compare X Register
class CPX : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("CPX");
};

// Compare Y Register
class CPY : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("CPY");
};

#endif  // ENGINE_INSTRUCTION_COMPARE_H_
