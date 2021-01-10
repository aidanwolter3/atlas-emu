#ifndef ENGINE_INSTRUCTION_STATUS_H_
#define ENGINE_INSTRUCTION_STATUS_H_

#include "src/engine/instruction/instruction.h"

// Clear Carry
class CLC : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("CLC");
};

// Set Carry
class SEC : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("SEC");
};

// Clear Interrupt
class CLI : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("CLI");
};

// Set Interrupt
class SEI : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("SEI");
};

// Clear Overflow
class CLV : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("CLV");
};

// Clear Decimal
class CLD : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("CLD");
};

// Set Decimal
class SED : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("SED");
};

#endif  // ENGINE_INSTRUCTION_STATUS_H_
