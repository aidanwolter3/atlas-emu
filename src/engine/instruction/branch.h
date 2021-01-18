#ifndef ENGINE_INSTRUCTION_BRANCH_H_
#define ENGINE_INSTRUCTION_BRANCH_H_

#include "src/engine/instruction/instruction.h"

class BPL : public Instruction {
 public:
  using Instruction::Instruction;
  uint8_t Execute(uint16_t operand) override;
  SET_LOG_NAME("BPL");
};

class BMI : public Instruction {
 public:
  using Instruction::Instruction;
  uint8_t Execute(uint16_t operand) override;
  SET_LOG_NAME("BMI");
};

class BVC : public Instruction {
 public:
  using Instruction::Instruction;
  uint8_t Execute(uint16_t operand) override;
  SET_LOG_NAME("BVC");
};

class BVS : public Instruction {
 public:
  using Instruction::Instruction;
  uint8_t Execute(uint16_t operand) override;
  SET_LOG_NAME("BVS");
};

class BCC : public Instruction {
 public:
  using Instruction::Instruction;
  uint8_t Execute(uint16_t operand) override;
  SET_LOG_NAME("BCC");
};

class BCS : public Instruction {
 public:
  using Instruction::Instruction;
  uint8_t Execute(uint16_t operand) override;
  SET_LOG_NAME("BCS");
};

class BNE : public Instruction {
 public:
  using Instruction::Instruction;
  uint8_t Execute(uint16_t operand) override;
  SET_LOG_NAME("BNE");
};

class BEQ : public Instruction {
 public:
  using Instruction::Instruction;
  uint8_t Execute(uint16_t operand) override;
  SET_LOG_NAME("BEQ");
};

#endif  // ENGINE_INSTRUCTION_BRANCH_H_
