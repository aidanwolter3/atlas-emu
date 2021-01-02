#ifndef ENGINE_INSTRUCTION_TRANSFER_H_
#define ENGINE_INSTRUCTION_TRANSFER_H_

#include "src/engine/public/instruction.h"

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

// Transfer X to stack pointer
class TXS : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("TXS");
};

// Transfer stack pointer to X
class TSX : public Instruction {
 public:
  using Instruction::Instruction;
  void ExecuteInternal(uint8_t opcode) override;
  SET_LOG_NAME("TSX");
};

#endif  // ENGINE_INSTRUCTION_TRANSFER_H_
