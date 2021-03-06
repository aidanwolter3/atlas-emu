#ifndef ENGINE_INSTRUCTION_LOGIC_H_
#define ENGINE_INSTRUCTION_LOGIC_H_

#include "src/engine/instruction/instruction.h"

// AND Accumulator
class AND : public Instruction {
 public:
  using Instruction::Instruction;
  ExecuteResult Execute(uint16_t operand) override;
  SET_LOG_NAME("AND");
};

// OR Accumulator
class ORA : public Instruction {
 public:
  using Instruction::Instruction;
  ExecuteResult Execute(uint16_t operand) override;
  SET_LOG_NAME("ORA");
};

// Exclusive OR Accumulator
class EOR : public Instruction {
 public:
  using Instruction::Instruction;
  ExecuteResult Execute(uint16_t operand) override;
  SET_LOG_NAME("EOR");
};

// Bit Test
class BIT : public Instruction {
 public:
  using Instruction::Instruction;
  ExecuteResult Execute(uint16_t operand) override;
  SET_LOG_NAME("BIT");
};

#endif  // ENGINE_INSTRUCTION_LOGIC_H_
