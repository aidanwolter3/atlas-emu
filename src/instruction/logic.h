#ifndef INSTRUCTION_LOGIC_H_
#define INSTRUCTION_LOGIC_H_

#include "src/public/instruction.h"

class AND : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("AND");
};

class ORA : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("ORA");
};

#endif  // INSTRUCTION_LOGIC_H_
