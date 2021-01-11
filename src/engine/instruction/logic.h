#ifndef ENGINE_INSTRUCTION_LOGIC_H_
#define ENGINE_INSTRUCTION_LOGIC_H_

#include "src/engine/instruction/instruction.h"

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

// Exclusive OR Accumulator
class EOR : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("EOR");
};

// Bit Test
class BIT : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  std::optional<uint8_t> Execute(uint8_t opcode, Instruction2::Mode mode,
                                 uint16_t operand, int cycle) override;
};

#endif  // ENGINE_INSTRUCTION_LOGIC_H_
