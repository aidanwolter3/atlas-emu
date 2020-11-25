#ifndef INSTRUCTION_REGISTER_H_
#define INSTRUCTION_REGISTER_H_

#include "src/public/instruction.h"

// Transfer A to X
class TAX : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("TAX");
};

// Transfer X to A
class TXA : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("TXA");
};

// Decrement X
class DEX : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("DEX");
};

// Increment X
class INX : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("INX");
};

// Transfer A to Y
class TAY : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("TAY");
};

// Transfer Y to A
class TYA : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("TYA");
};

// Decrement Y
class DEY : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("DEY");
};

// Increment Y
class INY : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("INY");
};

#endif  // INSTRUCTION_REGISTER_H_
