#ifndef ENGINE_INSTRUCTION_SHIFT_H_
#define ENGINE_INSTRUCTION_SHIFT_H_

#include "src/engine/instruction/instruction.h"

// Arithmetic Shift Left
class ASL : public Instruction {
 public:
  using Instruction::Instruction;
  uint8_t Execute(uint16_t operand) override;
};

// Logical Shift Right
class LSR : public Instruction {
 public:
  using Instruction::Instruction;
  uint8_t Execute(uint16_t operand) override;
};

// Rotate Left
class ROL : public Instruction {
 public:
  using Instruction::Instruction;
  uint8_t Execute(uint16_t operand) override;
};

// Rotate Right
class ROR : public Instruction {
 public:
  using Instruction::Instruction;
  uint8_t Execute(uint16_t operand) override;
};

#endif  // ENGINE_INSTRUCTION_SHIFT_H_
