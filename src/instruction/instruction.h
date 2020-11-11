#ifndef INSTRUCTION_INSTRUCTION_H_
#define INSTRUCTION_INSTRUCTION_H_

#include <cstdint>
#include <vector>

#include "src/memory.h"
#include "src/registers.h"

class Instruction {
 public:
  Instruction(Memory& mem, Registers& reg) : mem_(mem), reg_(reg) {}
  virtual ~Instruction() {}
  virtual void Execute(uint8_t opcode) = 0;

 protected:
  std::vector<uint8_t> FetchOperands(int num);

  Memory& mem_;
  Registers& reg_;
};

// No Operation
class NOP : public Instruction {
 public:
  using Instruction::Instruction;
  void Execute(uint8_t opcode) override {}
};

#endif  // INSTRUCTION_INSTRUCTION_H_
