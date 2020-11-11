#ifndef INSTRUCTION_LOAD_H_
#define INSTRUCTION_LOAD_H_

#include "src/public/instruction.h"

// Common class for shared methods.
class Load : public Instruction {
 public:
  using Instruction::Instruction;

 protected:
  // Set the zero and sign bits in the status register based on |data|.
  void SetStatusFromData(uint8_t data);

  // Common load methods that load the data into |dest|.
  // LDA #10
  void LoadImmediate(uint8_t& dest);
  // LDA $10
  void LoadZeroPage(uint8_t& dest);
  // LDA $10, X
  void LoadIndexedZeroPage(uint8_t& dest, uint8_t index);
  // LDA $1000
  void LoadAbsolute(uint8_t& dest);
  // LDA $1000, X
  void LoadIndexedAbsolute(uint8_t& dest, uint8_t index);
  // LDA ($10, X)
  void LoadIndexedIndirect(uint8_t& dest, uint8_t index);
  // LDA ($10), X
  void LoadIndirectIndexed(uint8_t& dest, uint8_t index);
};

// Load Accumulator
class LDA : public Load {
 public:
  using Load::Load;
  void Execute(uint8_t opcode) override;
};

// Load Index X
class LDX : public Load {
 public:
  using Load::Load;
  void Execute(uint8_t opcode) override;
};

// Load Index Y
class LDY : public Load {
 public:
  using Load::Load;
  void Execute(uint8_t opcode) override;
};

#endif  // INSTRUCTION_LOAD_H_
