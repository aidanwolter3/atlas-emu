#ifndef ENGINE_INSTRUCTION_MATH_H_
#define ENGINE_INSTRUCTION_MATH_H_

#include "src/engine/public/instruction.h"

// Add with Carry
class ADC : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("ADC");
};

// Subtract with Carry
class SBC : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("SBC");
};

// Decrement Memory
class DEC : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("DEC");
};

// Decrement X
class DEX : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("DEX");
};

// Decrement Y
class DEY : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("DEY");
};

// Increment Memory
class INC : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("INC");
};

// Increment X
class INX : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("INX");
};

// Increment Y
class INY : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("INY");
};

#endif  // ENGINE_INSTRUCTION_MATH_H_
