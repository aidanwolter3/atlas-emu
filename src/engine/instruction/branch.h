#ifndef ENGINE_INSTRUCTION_BRANCH_H_
#define ENGINE_INSTRUCTION_BRANCH_H_

#include "src/engine/instruction/instruction.h"

class BPL : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  std::optional<uint8_t> Execute(uint8_t opcode, Instruction2::Mode mode,
                                 uint16_t operand, int cycle) override;
};

class BMI : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  std::optional<uint8_t> Execute(uint8_t opcode, Instruction2::Mode mode,
                                 uint16_t operand, int cycle) override;
};

class BVC : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  std::optional<uint8_t> Execute(uint8_t opcode, Instruction2::Mode mode,
                                 uint16_t operand, int cycle) override;
};

class BVS : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  std::optional<uint8_t> Execute(uint8_t opcode, Instruction2::Mode mode,
                                 uint16_t operand, int cycle) override;
};

class BCC : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  std::optional<uint8_t> Execute(uint8_t opcode, Instruction2::Mode mode,
                                 uint16_t operand, int cycle) override;
};

class BCS : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  std::optional<uint8_t> Execute(uint8_t opcode, Instruction2::Mode mode,
                                 uint16_t operand, int cycle) override;
};

class BNE : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  std::optional<uint8_t> Execute(uint8_t opcode, Instruction2::Mode mode,
                                 uint16_t operand, int cycle) override;
};

class BEQ : public Instruction2 {
 public:
  using Instruction2::Instruction2;
  std::optional<uint8_t> Execute(uint8_t opcode, Instruction2::Mode mode,
                                 uint16_t operand, int cycle) override;
};

#endif  // ENGINE_INSTRUCTION_BRANCH_H_
