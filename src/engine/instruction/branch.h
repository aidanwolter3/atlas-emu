#ifndef ENGINE_INSTRUCTION_BRANCH_H_
#define ENGINE_INSTRUCTION_BRANCH_H_

#include "src/engine/instruction/instruction.h"

class BPL : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  uint8_t Execute(uint8_t opcode, uint16_t operand) override;
};

class BMI : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  uint8_t Execute(uint8_t opcode, uint16_t operand) override;
};

class BVC : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  uint8_t Execute(uint8_t opcode, uint16_t operand) override;
};

class BVS : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  uint8_t Execute(uint8_t opcode, uint16_t operand) override;
};

class BCC : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  uint8_t Execute(uint8_t opcode, uint16_t operand) override;
};

class BCS : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  uint8_t Execute(uint8_t opcode, uint16_t operand) override;
};

class BNE : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  uint8_t Execute(uint8_t opcode, uint16_t operand) override;
};

class BEQ : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  uint8_t Execute(uint8_t opcode, uint16_t operand) override;
};

#endif  // ENGINE_INSTRUCTION_BRANCH_H_
