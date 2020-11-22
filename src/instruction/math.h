#ifndef INSTRUCTION_MATH_H_
#define INSTRUCTION_MATH_H_

#include "src/public/instruction.h"

class ADC : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("ADC");
};

class SBC : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("SBC");
};

#endif  // INSTRUCTION_MATH_H_
