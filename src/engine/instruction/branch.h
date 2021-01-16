#ifndef ENGINE_INSTRUCTION_BRANCH_H_
#define ENGINE_INSTRUCTION_BRANCH_H_

#include "src/engine/instruction/instruction.h"

class BPL : public Instruction {
 public:
  using Instruction::Instruction;
  uint8_t Execute(uint16_t operand) override;
};

class BMI : public Instruction {
 public:
  using Instruction::Instruction;
  uint8_t Execute(uint16_t operand) override;
};

class BVC : public Instruction {
 public:
  using Instruction::Instruction;
  uint8_t Execute(uint16_t operand) override;
};

class BVS : public Instruction {
 public:
  using Instruction::Instruction;
  uint8_t Execute(uint16_t operand) override;
};

class BCC : public Instruction {
 public:
  using Instruction::Instruction;
  uint8_t Execute(uint16_t operand) override;
};

class BCS : public Instruction {
 public:
  using Instruction::Instruction;
  uint8_t Execute(uint16_t operand) override;
};

class BNE : public Instruction {
 public:
  using Instruction::Instruction;
  uint8_t Execute(uint16_t operand) override;
};

class BEQ : public Instruction {
 public:
  using Instruction::Instruction;
  uint8_t Execute(uint16_t operand) override;
};

#endif  // ENGINE_INSTRUCTION_BRANCH_H_
