#ifndef INSTRUCTION_LOGIC_H_
#define INSTRUCTION_LOGIC_H_

#include "src/public/instruction.h"

// AND Accumulator
class AND : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("AND");
};

// OR Accumulator
class ORA : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("ORA");
};

// Bit Test
class BIT : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("BIT");
};

#endif  // INSTRUCTION_LOGIC_H_
