#ifndef INSTRUCTION_SHIFT_H_
#define INSTRUCTION_SHIFT_H_

#include "src/public/instruction.h"

// Arithmetic Shift Left
class ASL : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("ASL");
};

// Logical Shift Right
class LSR : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("LSR");
};

// Rotate Left
class ROL : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("ROL");
};

// Rotate Right
class ROR : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("ROR");
};

#endif  // INSTRUCTION_SHIFT_H_
